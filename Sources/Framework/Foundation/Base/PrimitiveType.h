//***********************************************************
//! @file
//! @brief		プリミティブ型定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <stdint.h>
#include <stddef.h>
#include "Platform.h"

namespace ob
{

    // ============================================
    // 基本型
    // ============================================

    using s8    = int8_t;	//!<  8ビット符号付き整数
    using s16   = int16_t;	//!< 16ビット符号付き整数
    using s32   = int32_t;	//!< 32ビット符号付き整数
    using s64   = int64_t;	//!< 64ビット符号付き整数
    using u8    = uint8_t;	//!<  8ビット符号無し整数
    using u16   = uint16_t;	//!< 16ビット符号無し整数
    using u32   = uint32_t;	//!< 32ビット符号無し整数
    using u64   = uint64_t;	//!< 64ビット符号無し整数

    using f32   = float;	//!< 32ビット浮動小数点
    using f64   = double;	//!< 64ビット浮動小数点

    using b8    = int8_t;	//!<  8ビット真偽値
    using b32   = int32_t;	//!< 32ビット真偽値


    using byte  = u8;	    //!< バイト値


    using Char = char16_t;  //!< 文字型

#undef TC
#undef TC_BASE
    //! @cond
#define TC_BASE(x) u ## x
    //! @endcond
#define TC(x) TC_BASE(x)
    //@―---------------------------------------------------------------------------
    //! @def        TC
    //! @brief      文字/文字列リテラルをエンジン既定のエンコーディングに変更する。
    //@―---------------------------------------------------------------------------


#if defined(OS_64BIT)
    using addr_t    = u64;
    using offset_t  = s64;
#elif defined(OS_32BIT)
    using addr_t    = u32;
    using offset_t  = s32;
#endif

    //@―---------------------------------------------------------------------------
    //! @typedef    addr_t
    //! @brief      アドレス型
    //! @details    アドレスに対して演算処理をする場合に使用。
    //!             ```
    //!             // アライメントされたアドレスの計算
    //!             const s32 alignment=32;
    //!             byte bytes[128];
    //!             addr_t address=static_cast<addr_t>(bytes);
    //!             offset_t offset=-(alignment-address%alignment)%alignment;
    //!             address=address+offset;
    //!             ```
    //@―---------------------------------------------------------------------------
    //! @typedef    offset_t
    //! @brief      アドレス差分型
    //! @details    アドレスに対して負の方向の差分を表すことが可能。
    //!             ```
    //!             // アライメントされたアドレスの計算
    //!             const s32 alignment=32;
    //!             byte bytes[128];
    //!             addr_t address=static_cast<addr_t>(bytes);
    //!             offset_t offset=-(alignment-address%alignment)%alignment;
    //!             address=address+offset;
    //!             ```
    //@―---------------------------------------------------------------------------

}// namespcae ob