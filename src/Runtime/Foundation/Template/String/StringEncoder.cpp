//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include"StringEncoder.h"
namespace ob
{

    //@―---------------------------------------------------------------------------
    //! @brief  説明
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
}// namespace ob