//***********************************************************
//! @file
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Misc/UUID.h>
#include <Framework/Core/Utility/Indexed.h>

namespace ob::core {


    byte DigitCharToHex(int ch) {
        if ('0' <= ch && ch <= '9') return static_cast<byte>(ch - 48);
        if ('a' <= ch && ch <= 'f') return static_cast<byte>(ch - 87);
        if ('A' <= ch && ch <= 'F') return static_cast<byte>(ch - 55);
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


    //! @brief			UUID文字列からUUIDオブジェクトを生成
    //! 
    //! @details		XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される16進数の並び
    //! @param uuidText UUIDの文字列表現
    Optional<UUID> UUID::FromString(const StringView& uuidText) {

        if (uuidText.size() != 32 + 3)return std::nullopt;

        UUID result;
        auto ptr = reinterpret_cast<u8*>(&result.data);
        for (auto [i,c] : Indexed(uuidText)) {
            if (c == '-') {
                if (i == 8 || i == 12 || i == 17 || i == 22) {
                    continue;
                } else if(36<i){
                    return std::nullopt;
                }
                if ('0' <= c && c <= '9')(*ptr) = c - '0';
                if ('a' <= c && c <= 'f')(*ptr) = c - 'a';
                if ('A' <= c && c <= 'F')(*ptr) = c - 'A';
                else return std::nullopt;
                ++ptr;
            }
        }

        return result;
    }

    //! @brief 文字列表現で取得
    //! 
    //! @return XXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される文字列表現
    void UUID::toString(Char(&dest)[37])const {
        FormatTo(dest,"{:08x}-{:04x}-{:04x}-{:04x}-{:04x}{:08x}", data[0], data[1] >> 16, data[1] & 0xFFFF, data[2] >> 16, data[2] & 0xFFFF, data[3]);
        dest[36] = '\0';
    }


    //! @brief 文字列表現で取得
    //! 
    //! @return XXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される文字列表現
    String UUID::toString() const {
        Char text[37];
        toString(text);
        return String(text);
    }

} 