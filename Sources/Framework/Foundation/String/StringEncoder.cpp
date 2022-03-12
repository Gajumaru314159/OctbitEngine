//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include"StringEncoder.h"

namespace ob::foundation {

    //@―---------------------------------------------------------------------------
    //! @brief  char16_t から wchar_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(const StringBase<char16_t>& src, StringBase<wchar_t>& dest) {
        if (sizeof(char16_t) == sizeof(wchar_t)) {
            dest.clear();
            dest.reserve(src.size());
            auto itrS = src.begin();
            while (itrS != src.end()) {
                dest.push_back(static_cast<wchar_t>(*itrS));
                ++itrS;
            }
        } else {
            dest.clear();
            dest.reserve(src.size());
            auto itrS = src.begin();
            while (itrS != src.end()) {
                u32 high = static_cast<u32>(*itrS);
                if (is_in_range(high, 0xD800U, 0xDC00U - 1)) {
                    u32 low = *(++itrS);
                    if (is_in_range(low, 0xDC00U, 0xE000U - 1)) {
                        dest.push_back(0x10000U + (high - 0xD800) * 0x400 + (low - 0xDC00));
                    } else if (low == 0) {
                        dest.push_back(high);
                    } else {
                        return false;
                    }
                    if (itrS == src.end())continue;
                }
                ++itrS;
            }
        }
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  wchar_t から char16_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(const StringBase<wchar_t>& src, StringBase<char16_t>& dest) {
        if (sizeof(char16_t) == sizeof(wchar_t)) {
            dest.clear();
            dest.reserve(src.size());
            auto itrS = src.begin();
            while (itrS != src.end()) {
                dest.push_back(static_cast<wchar_t>(*itrS));
                ++itrS;
            }
        } else {
            dest.clear();
            dest.reserve(src.size() * 2);

            auto itrS = src.begin();
            while (itrS != src.end()) {
                u32 c = static_cast<u32>(*itrS);

                if (c < 0x10000) {
                    dest.push_back(c);
                } else if (0 <= c) {
                    dest.push_back(char16_t((c - 0x10000) / 0x400 + 0xD800));
                    dest.push_back(char16_t((c - 0x10000) % 0x400 + 0xDC00));
                } else {
                    return false;
                }

                ++itrS;
            }
        }
        return true;
    }
}// namespace ob::foundation