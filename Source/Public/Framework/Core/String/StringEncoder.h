﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/String/String.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief      文字列変換
    //! 
    //! @details    char / wchar_t / char16_t の相互変換をします。
    //!             StringBase<char> のエンコーディングはUTF-8である必要があります。
    //!             StringBase<char16_t> のエンコーディングはUTF-16である必要があります。
    //!             StringBase<char32_t> のエンコーディングはUTF-32である必要があります。
    //!             StringBase<wchar_t> のエンコーディングはUTF-8かUTF-16かUTF-32である必要があります。
    //!             Char は上記型のいずれかのエイリアスです。
    //!             任意の型の文字列と String を相互に変換可能です。
    //@―---------------------------------------------------------------------------
    class OB_API StringEncoder {
    public:

        //===============================================================
        // char
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  char から char に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char> src, StringBase<char>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  char から char16_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char> src, StringBase<char16_t>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  char から char32_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char> src, StringBase<char32_t>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  char から wchar_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char> src, StringBase<wchar_t>& dest);



        //===============================================================
        // char16_t
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  char16_t から char に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char16_t> src, StringBase<char>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  char16_t から char16_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char16_t> src, StringBase<char16_t>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  char16_t から char32_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char16_t> src, StringBase<char32_t>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  char16_t から wchar_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char16_t> src, StringBase<wchar_t>& dest);



        //===============================================================
        // char32_t
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  char32_t から char に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char32_t> src, StringBase<char>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  char32_t から char16_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char32_t> src, StringBase<char16_t>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  char32_t から char32_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char32_t> src, StringBase<char32_t>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  char32_t から wchar_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<char32_t> src, StringBase<wchar_t>& dest);



        //===============================================================
        // wchar_t
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  wchar_t から char に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<wchar_t> src, StringBase<char>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  wchar_t から char16_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<wchar_t> src, StringBase<char16_t>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  wchar_t から char32_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<wchar_t> src, StringBase<char32_t>& dest);


        //@―---------------------------------------------------------------------------
        //! @brief  wchar_t から wchar_t に変換する
        //@―---------------------------------------------------------------------------
        static bool Encode(StringViewBase<wchar_t> src, StringBase<wchar_t>& dest);

    };

}// namespcae ob