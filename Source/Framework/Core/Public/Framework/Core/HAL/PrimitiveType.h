//***********************************************************
//! @file
//! @brief		プリミティブ型定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <Framework/Core/HAL/Platform.h>

namespace ob::core {

    //============================================
    // 基本型
    //============================================

    using s8  = int8_t;	    //!<  8ビット符号付き整数
    using s16 = int16_t;	//!< 16ビット符号付き整数
    using s32 = int32_t;	//!< 32ビット符号付き整数
    using s64 = int64_t;	//!< 64ビット符号付き整数
    using u8  = uint8_t;	//!<  8ビット符号無し整数
    using u16 = uint16_t;	//!< 16ビット符号無し整数
    using u32 = uint32_t;	//!< 32ビット符号無し整数
    using u64 = uint64_t;	//!< 64ビット符号無し整数

    using f32 = float;	    //!< 32ビット浮動小数点
    using f64 = double;	    //!< 64ビット浮動小数点

    using byte = u8;	    //!< バイト値


    using Char = char16_t;  //!< 文字型

#undef TC
    //! @cond
#define _internal_TC(x) u ## x
    //! @endcond
#define TC(x) _internal_TC(x)
    //@―---------------------------------------------------------------------------
    //! @def        TC
    //! @brief      文字/文字列リテラルをエンジン既定のエンコーディングに変更する。
    //@―---------------------------------------------------------------------------


#if defined(OS_64BIT)
    using addr_t   = u64;   //!< アドレス型
    using offset_t = s64;   //!< アドレス・オフセット型
#elif defined(OS_32BIT)
    using addr_t   = u32;   //!< アドレス型
    using offset_t = s32;   //!< アドレス・オフセット型
#endif


    //============================================
    // 型チェック
    //============================================

    static_assert(sizeof(s8) == 1, "s8 type size test failed.");
    static_assert(sizeof(u8) == 1, "u8 type size test failed.");
    static_assert(sizeof(s16) == 2, "s16 type size test failed.");
    static_assert(sizeof(u16) == 2, "u16 type size test failed.");
    static_assert(sizeof(s32) == 4, "s32 type size test failed.");
    static_assert(sizeof(u32) == 4, "u32 type size test failed.");
    static_assert(sizeof(s64) == 8, "s64 type size test failed.");
    static_assert(sizeof(u64) == 8, "u64 type size test failed.");

    static_assert(sizeof(f32) == 4, "f32 type size test failed.");
    static_assert(sizeof(f64) == 8, "f64 type size test failed.");

    static_assert(sizeof(byte) == 1, "byte type size test failed.");

}// namespcae ob