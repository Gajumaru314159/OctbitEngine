//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <stdint.h>

namespace ob
{

    // ============================================
     // 基本型
     // ============================================

    using s8 = int8_t;		//!<  8ビット符号付き整数
    using s16 = int16_t;	//!< 16ビット符号付き整数
    using s32 = int32_t;	//!< 32ビット符号付き整数
    using s64 = int64_t;	//!< 64ビット符号付き整数
    using u8 = uint8_t;		//!<  8ビット符号無し整数
    using u16 = uint16_t;	//!< 16ビット符号無し整数
    using u32 = uint32_t;	//!< 32ビット符号無し整数
    using u64 = uint64_t;	//!< 64ビット符号無し整数

    using f32 = float;		//!< 32ビット浮動小数点
    using f64 = double;		//!< 64ビット浮動小数点

    using b8 = int8_t;		//!<  8ビット真偽値
    using b32 = int32_t;	//!< 32ビット真偽値

    using Size = size_t;	//!< アドレスなどのサイズ型

#define TRUE (1)			//!< 真
#define FALSE (0)			//!< 偽

    using Byte = uint8_t;	//!< バイト値

}// namespcae ob