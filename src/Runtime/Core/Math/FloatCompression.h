﻿//***********************************************************
//! @file
//! @brief		浮動小数の圧縮
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Math/Vector/include.h>


namespace ob {

    //@―---------------------------------------------------------------------------


    class FloatCompression {
    public:

        static u32 F32ToU16(f32 value);     // f32 を 16bit小数に圧縮
        static f32 U16ToF32(u16 value);     // 16bit小数を f32 に展開

        //static u8  F32ToU8(f32 value);     // f32(-1.0f～1.0f) を u8 に圧縮
        //static f32 U8ToF32(u8  value);     // u8 を f32(-1.0f～1.0f) に展開

        //static u32 F32ToF11(f32 value);    // f32 を11bitに圧縮
        //static f32 F11ToF32(u32  value);   // 11bitを f32 に展開

        //static u32 F32ToF10(f32 value);    // f32 を11bitに圧縮
        //static f32 F10ToF32(u32  value);   // 11bitを f32 に展開

        //static u32  Vec3ToU32(const Vec3& value);   // Vec3 を[11,11,10]の32bitに圧縮
        //static Vec3 U32ToVec3(u32  value);          // [11,11,10]の32bitを Vec3 に展開

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------


}// namespcae ob