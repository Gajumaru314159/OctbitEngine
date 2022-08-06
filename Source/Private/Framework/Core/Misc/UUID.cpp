//***********************************************************
//! @file
//! @brief UUID
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Misc/UUID.h>

#include <objbase.h>

#include <Framework/Core/Misc/CRCHash.h>


namespace ob::core {


    byte DigitCharToHex(int ch) {
        if ('0' <= ch && ch <= '9') return ch - 48;
        if ('a' <= ch && ch <= 'f') return ch - 87;
        if ('A' <= ch && ch <= 'F') return ch - 55;
        return 0;
    }

    byte CharPairToHex(int a, int b) {
        return DigitCharToHex(a) * 16 + DigitCharToHex(b);
    }

    Char HexToDigitChar(byte hex) {
        if (hex >= 10) return 'a' + (hex - 10);
        return '0' + hex;
    }

    void HexToCharPair(byte hex, Char* ch1, Char* ch2) {
        *ch1 = HexToDigitChar((hex >> 4) & 0x0F);
        *ch2 = HexToDigitChar((hex) & 0x0F);
    }



    //==============================================================================
    // Uuid
    //==============================================================================

    //@―---------------------------------------------------------------------------
    //! @brief 新しいUUIDを生成
    //@―---------------------------------------------------------------------------
    UUID UUID::Generate() {
        GUID guid;
        HRESULT r = CoCreateGuid(&guid);

        UUID result;
        result.m_data[0] = (byte)((guid.Data1 >> 24) & 0xFF);
        result.m_data[1] = (byte)((guid.Data1 >> 16) & 0xFF);
        result.m_data[2] = (byte)((guid.Data1 >> 8) & 0xFF);
        result.m_data[3] = (byte)((guid.Data1) & 0xFF);
        result.m_data[4] = (byte)((guid.Data2 >> 8) & 0xFF);
        result.m_data[5] = (byte)((guid.Data2) & 0xFF);
        result.m_data[6] = (byte)((guid.Data3 >> 8) & 0xFF);
        result.m_data[7] = (byte)((guid.Data3) & 0xFF);
        result.m_data[8] = guid.Data4[0];
        result.m_data[9] = guid.Data4[1];
        result.m_data[10] = guid.Data4[2];
        result.m_data[11] = guid.Data4[3];
        result.m_data[12] = guid.Data4[4];
        result.m_data[13] = guid.Data4[5];
        result.m_data[14] = guid.Data4[6];
        result.m_data[15] = guid.Data4[7];

        return result;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			UUID文字列からUUIDオブジェクトを生成
    //! 
    //! @details		XXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される16進数の並び
    //! @param uuidText UUIDの文字列表現
    //@―---------------------------------------------------------------------------
    UUID UUID::FromText(const StringView& uuidText) {
        Char ch1;
        bool lookingCh1 = true;

        UUID result;
        s32 idx = 0;
        for (s32 i = 0; i < (s32)uuidText.length(); i++) {
            Char ch = uuidText[i];
            if (ch == TC('-') || ch == TC('{') || ch == TC('}') || ch == TC('(') || ch == TC(')')) continue;

            if (lookingCh1) {
                ch1 = ch;
                lookingCh1 = false;
            } else {
                result.m_data[idx++] = CharPairToHex(ch1, ch);
                lookingCh1 = true;
            }
        }
        return result;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			バイト列から固有のUUIDを生成
    //! @param bytes	バイト列
    //@―---------------------------------------------------------------------------
    UUID UUID::FromByte(const byte* bytes) {
        OB_CHECK_ASSERT_EXPR(bytes != nullptr);
        UUID result;
        for (s32 i = 0; i < 16; i++) {
            result.m_data[i] = *(bytes + i);
        }
        return result;
    }


    //@―---------------------------------------------------------------------------
    //! @brief デフォルトコンストラクタ
    //@―---------------------------------------------------------------------------
    UUID::UUID() {
        memset(m_data, 0, sizeof(m_data));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 文字列表現で取得
    //! 
    //! @return XXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される文字列表現
    //@―---------------------------------------------------------------------------
    void UUID::toString(Char(&dest)[37])const {
        Char* str = dest;
        HexToCharPair(m_data[0], &str[0], &str[1]);
        HexToCharPair(m_data[1], &str[2], &str[3]);
        HexToCharPair(m_data[2], &str[4], &str[5]);
        HexToCharPair(m_data[3], &str[6], &str[7]);
        str[8] = TC('-');
        HexToCharPair(m_data[4], &str[9], &str[10]);
        HexToCharPair(m_data[5], &str[11], &str[12]);
        str[13] = TC('-');
        HexToCharPair(m_data[6], &str[14], &str[15]);
        HexToCharPair(m_data[7], &str[16], &str[17]);
        str[18] = TC('-');
        HexToCharPair(m_data[8], &str[19], &str[20]);
        HexToCharPair(m_data[9], &str[21], &str[22]);
        str[23] = TC('-');
        HexToCharPair(m_data[10], &str[24], &str[25]);
        HexToCharPair(m_data[11], &str[26], &str[27]);
        HexToCharPair(m_data[12], &str[28], &str[29]);
        HexToCharPair(m_data[13], &str[30], &str[31]);
        HexToCharPair(m_data[14], &str[32], &str[33]);
        HexToCharPair(m_data[15], &str[34], &str[35]);
        str[36] = TC('\0');
    }


    //@―---------------------------------------------------------------------------
    //! @brief 文字列表現で取得
    //! 
    //! @return XXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される文字列表現
    //@―---------------------------------------------------------------------------
    String UUID::toString() const {
        Char text[37];
        toString(text);
        text[36] = TC('\0');
        return String(text);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 空のUUIDか判定
    //! 
    //! @retval true	空
    //! @retval false	空ではない
    //@―---------------------------------------------------------------------------
    bool UUID::empty() const {
        for (auto i : m_data) {
            if (i != 0) return false;
        }
        return true;
    }

} // namespace ob