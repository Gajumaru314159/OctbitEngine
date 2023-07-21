//***********************************************************
//! @file
//! @brief UUID
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Misc/UUID.h>
#include <Framework/Core/Utility/Indexed.h>

#ifdef OS_WINDOWS
#include <objbase.h>
#endif

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
        UUID uuid;
#ifdef OS_WINDOWS
        HRESULT r = CoCreateGuid(reinterpret_cast<GUID*>(&uuid));
        OB_ASSERT(r == S_OK, "Failed to create UUID.");
#else
#pragma error("UUID::Generate() is not supported in this platform.")
#endif
        return uuid;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			UUID文字列からUUIDオブジェクトを生成
    //! 
    //! @details		XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される16進数の並び
    //! @param uuidText UUIDの文字列表現
    //@―---------------------------------------------------------------------------
    Optional<UUID> UUID::FromString(const StringView& uuidText) {

        if (uuidText.size() != 32 + 3)return std::nullopt;

        UUID result;
        auto ptr = reinterpret_cast<u8*>(&result.data);
        for (auto& [i,c] : Indexed(uuidText)) {
            if (c == TC('-')) {
                if (i == 8 || i == 12 || i == 17 || i == 22) {
                    continue;
                } else {
                    return std::nullopt;
                }
                if (TC('0') <= c && c <= TC('9'))(*ptr) = c - '0';
                if (TC('a') <= c && c <= TC('f'))(*ptr) = c - 'a';
                if (TC('A') <= c && c <= TC('F'))(*ptr) = c - 'A';
                else return std::nullopt;
                ++ptr;
            }
        }

        return result;
    }

    //@―---------------------------------------------------------------------------
    //! @brief 文字列表現で取得
    //! 
    //! @return XXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される文字列表現
    //@―---------------------------------------------------------------------------
    void UUID::toString(Char(&dest)[37])const {
        FormatTo(dest,TC("{:08x}-{:04x}-{:04x}-{:04x}-{:04x}{:08x}"), data[0], data[1] >> 16, data[1] & 0xFFFF, data[2] >> 16, data[2] & 0xFFFF, data[3]);
        dest[36] = TC('\0');
    }


    //@―---------------------------------------------------------------------------
    //! @brief 文字列表現で取得
    //! 
    //! @return XXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される文字列表現
    //@―---------------------------------------------------------------------------
    String UUID::toString() const {
        Char text[37];
        toString(text);
        return String(text);
    }

} 