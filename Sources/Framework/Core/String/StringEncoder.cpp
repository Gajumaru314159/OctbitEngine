//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include"StringEncoder.h"
#include <utf8.h>

namespace {
    //! @cond 
    // 文字列変換時に変換後の文字サイズを計算するのが面倒なためおおよその数をreserveするのに使用
    // 文字数を計算するほうがパフォーマンスがいいのであれば対応

    inline size_t size8to16(size_t size) {
        return size * 70 / 100;
    }

    inline size_t size8to32(size_t size) {
        return size * 70 / 100;
    }

    inline size_t size16to8(size_t size) {
        return size * 100 / 70;
    }

    inline size_t size16to32(size_t size) {
        return size;
    }

    inline size_t size32to8(size_t size) {
        return size * 100 / 70;
    }

    inline size_t size32to16(size_t size) {
        return size;
    }

    //! @endcond 
}

namespace ob::core {

#pragma region char

    //@―---------------------------------------------------------------------------
    //! @brief  char から char に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char> src, StringBase<char>& dest) {
        dest = src;
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char から char16_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char> src, StringBase<char16_t>& dest) {
        dest.clear();
        try {
            dest = utf8::utf8to16(src);
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char から char32_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char> src, StringBase<char32_t>& dest) {
        dest.clear();
        try {
            dest = utf8::utf8to32(src);
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char から wchar_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char> src, StringBase<wchar_t>& dest) {
        dest.clear();
        try {
            if (sizeof(wchar_t) == sizeof(char)) {
                dest.resize(src.size());
                for (s32 i = 0; i < dest.size(); ++i)dest[i] = src[i];
            }
            if (sizeof(wchar_t) == sizeof(char16_t)) {
                dest.reserve(size8to16(src.size()));
                utf8::utf8to16(src.begin(), src.end(), back_inserter(dest));
            }
            if (sizeof(wchar_t) == sizeof(char32_t)) {
                dest.reserve(size8to32(src.size()));
                utf8::utf8to32(src.begin(), src.end(), back_inserter(dest));
            }
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

#pragma endregion

#pragma region char16_t

    //@―---------------------------------------------------------------------------
    //! @brief  char16_t から char に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char16_t> src, StringBase<char>& dest) {
        dest.clear();
        try {
            dest = utf8::utf16to8(src);
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char16_t から char16_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char16_t> src, StringBase<char16_t>& dest) {
        dest = src;
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char16_t から char32_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char16_t> src, StringBase<char32_t>& dest) {
        dest.clear();
        try {
            auto tmp = utf8::utf16to8(src);
            dest = utf8::utf8to32(tmp);
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char16_t から wchar_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char16_t> src, StringBase<wchar_t>& dest) {
        dest.clear();
        try {
            if (sizeof(wchar_t) == sizeof(char)) {
                dest.reserve(size16to8(src.size()));
                utf8::utf16to8(src.begin(), src.end(), back_inserter(dest));
            }
            if (sizeof(wchar_t) == sizeof(char16_t)) {
                dest.resize(src.size());
                for (s32 i = 0; i < dest.size(); ++i)dest[i] = src[i];
            }
            if (sizeof(wchar_t) == sizeof(char32_t)) {
                dest.reserve(size16to32(src.size()));
                auto tmp = utf8::utf16to8(src);
                utf8::utf8to32(tmp.begin(),tmp.end(),back_inserter(dest));
            }
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

#pragma endregion

#pragma region char32_t

    //@―---------------------------------------------------------------------------
    //! @brief  char32_t から char に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char32_t> src, StringBase<char>& dest) {
        dest.clear();
        try {
            dest = utf8::utf32to8(src);
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char32_t から char16_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char32_t> src, StringBase<char16_t>& dest) {
        dest.clear();
        dest.reserve(src.size() * 110 / 100);
        try {
            auto tmp = utf8::utf32to8(src);
            dest = utf8::utf8to16(tmp);
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char32_t から char32_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char32_t> src, StringBase<char32_t>& dest) {
        dest = src;
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char32_t から wchar_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char32_t> src, StringBase<wchar_t>& dest) {
        dest.clear();
        try {
            if (sizeof(wchar_t) == sizeof(char)) {
                dest.reserve(size32to8(src.size()));
                utf8::utf32to8(src.begin(), src.end(), back_inserter(dest));
            }
            if (sizeof(wchar_t) == sizeof(char16_t)) {
                dest.reserve(size32to16(src.size()));
                auto tmp = utf8::utf32to8(src);
                utf8::utf8to16(tmp.begin(), tmp.end(), back_inserter(dest));
            }
            if (sizeof(wchar_t) == sizeof(char32_t)) {
                dest.resize(src.size());
                for (s32 i = 0; i < dest.size(); ++i)dest[i] = static_cast<wchar_t>(src[i]);
            }
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

#pragma endregion

#pragma region wchar_t

    //@―---------------------------------------------------------------------------
    //! @brief  wchar_t から char に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<wchar_t> src, StringBase<char>& dest) {
        dest.clear();
        try {
            if (sizeof(wchar_t) == sizeof(char)) {
                dest.resize(src.size());
                for (s32 i = 0; i < dest.size(); ++i)dest[i] = static_cast<char>(src[i]);
            }
            if (sizeof(wchar_t) == sizeof(char16_t)) {
                dest.reserve(size16to8(src.size()));
                utf8::utf16to8(src.begin(), src.end(), back_inserter(dest));
            }
            if (sizeof(wchar_t) == sizeof(char32_t)) {
                dest.reserve(size32to8(src.size()));
                utf8::utf32to8(src.begin(), src.end(), back_inserter(dest));
            }
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  wchar_t から char16_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<wchar_t> src, StringBase<char16_t>& dest) {
        dest.clear();
        try {
            if (sizeof(wchar_t) == sizeof(char)) {
                return false;
                //dest.reserve(size8to16(src.size()));
                //utf8::utf8to16(src.begin(), src.end(), back_inserter(dest));
            }
            if (sizeof(wchar_t) == sizeof(char16_t)) {
                dest.resize(src.size());
                for (s32 i = 0; i < dest.size(); ++i)dest[i] = src[i];
            }
            if (sizeof(wchar_t) == sizeof(char32_t)) {
                dest.reserve(size32to16(src.size()));
                utf8::utf32to8(src.begin(), src.end(), back_inserter(dest));
            }
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  wchar_t から char32_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<wchar_t> src, StringBase<char32_t>& dest) {
        dest.clear();
        try {
            if (sizeof(wchar_t) == sizeof(char)) {
                return false;
                //dest.reserve(size8to32(src.size()));
                //utf8::utf8to16(src.begin(), src.end(), back_inserter(dest));
            }
            if (sizeof(wchar_t) == sizeof(char16_t)) {
                StringBase<char> tmp;
                tmp.resize(size16to8(src.size()));
                utf8::utf16to8(src.begin(), src.end(), back_inserter(dest));
                dest = utf8::utf8to32(tmp);
            }
            if (sizeof(wchar_t) == sizeof(char32_t)) {
                dest.resize(src.size());
                for (s32 i = 0; i < dest.size(); ++i)dest[i] = src[i];
            }
        } catch (utf8::exception) {
            dest.clear();
            return false;
        }
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  wchar_t から wchar_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<wchar_t> src, StringBase<wchar_t>& dest) {
        dest = src;
        return true;
    }

#pragma endregion



    /*
#pragma region wchar_t

    //@―---------------------------------------------------------------------------
    //! @brief  char から char に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<wchar_t> src, StringBase<char>& dest) {
        if (sizeof(wchar_t) == sizeof(char16_t)) {

            utf8::utf8to16(src.begin(), src.end(), back_inserter(dest));

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

    //@―---------------------------------------------------------------------------
    //! @brief  char から wchar_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<wchar_t> src, StringBase<wchar_t>& dest) {
        dest.clear();
        try {
            auto converted = utf8::utf8to16(src);
            dest.resize(converted.size());

            for (s32 i = 0; i < src.size(); ++i) {
                dest[i] = converted[i];
            }

            return true;
        } catch (utf8::exception) {
        }
        return false;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char から wchar_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<wchar_t> src, StringBase<char16_t>& dest) {
        dest.clear();
        try {
            dest = utf8::utf8to16(src);
            return true;
        } catch (utf8::exception) {
        }
        return false;
    }

#pragma endregion

#pragma region char

    //@―---------------------------------------------------------------------------
    //! @brief  char から char に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char> src, StringBase<char>& dest) {
        dest = src;
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char から wchar_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char> src, StringBase<wchar_t>& dest) {
        dest.clear();
        try {
            auto converted = utf8::utf8to16(src);
            dest.resize(converted.size());

            for (s32 i = 0; i < src.size(); ++i) {
                dest[i] = converted[i];
            }

            return true;
        } catch (utf8::exception) {
        }
        return false;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  char から wchar_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char> src, StringBase<char16_t>& dest) {
        dest.clear();
        try {
            dest = utf8::utf8to16(src);
            return true;
        } catch (utf8::exception) {
        }
        return false;
    }

#pragma endregion


    //@―---------------------------------------------------------------------------
    //! @brief  char16_t から wchar_t に変換する
    //@―---------------------------------------------------------------------------
    bool StringEncoder::Encode(StringViewBase<char16_t> src, StringBase<wchar_t>& dest) {
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
    bool StringEncoder::Encode(StringViewBase<wchar_t> src, StringBase<char16_t>& dest) {
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
    */
}// namespace ob::core