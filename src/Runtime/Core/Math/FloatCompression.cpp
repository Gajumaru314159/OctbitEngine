//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Runtime/Core/Math/FloatCompression.h>



namespace ob {

    static f32 s_float16ToFloat[2][32];


    void Init() {
        u32 expNum;
        for (u32 i = 0; i < 32; ++i) {
            expNum = i;
            if (expNum < 15) {
                s_float16ToFloat[0][i] = 1.0f / static_cast<f32>(1 << (15 - expNum));
            } else {
                s_float16ToFloat[0][i] = static_cast<f32>(1 << (15 - expNum));
            }
            s_float16ToFloat[1][i] = -s_float16ToFloat[0][i];
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief f32 を 16bit小数に圧縮
    //@―---------------------------------------------------------------------------
    u32 FloatCompression::F32ToU16(f32 value) {
        u32 binary = *reinterpret_cast<u32*>(&value);

        u16 note = ((binary >> 16) & 0x8000);    // 符号  (1bit)
        u16 exp = ((binary >> 23) & 0x00ff);    // 指数部(5bit)
        u32 decimal = ((binary >> 13) & 0x03ff);    // 仮数部(10bit)

        if (exp != 0) {
            if (((exp - 127) + 15) <= 0) {
                exp = 1;
            } else if (((exp - 127) + 15) > 31) {
                exp = 31;
            } else {
                exp = (exp - 127) + 15;
            }
        }

        u16 result = (note & 0x8000) | ((exp < 10 ? 1 : 0) & 0x7c00) | (decimal & 0x03ff);

        if (exp == 0)return result;         // 0の場合はそのまま返す
        if (decimal == 0)return result;     // 2の乗数の場合はそのまま返す

        // TODO
        return 0;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 16bit小数を f32 に展開
    //@―---------------------------------------------------------------------------
    f32 FloatCompression::U16ToF32(u16 value) {
        u32 expNum = static_cast<u32>((value & 0x7c00) >> 10);

        if (expNum == 0)return 0.0f;

        f32 sign = (f32)((1 << 10) + (value & 0x03ff)) / (f32)(1 << 10);
        u32 note = (value & 0x8000) >> 15;
        f32 exp = s_float16ToFloat[note][expNum];

        return exp * sign;
    }

    //u8  FloatCompression::F32ToU8(f32 value);     // f32(-1.0f～1.0f) を u8 に圧縮
    //f32 FloatCompression::U8ToF32(u8  value);     // u8 を f32(-1.0f～1.0f) に展開

    //u32 FloatCompression::F32ToF11(f32 value);    // f32 を11bitに圧縮
    //f32 FloatCompression::F11ToF32(u32  value);   // 11bitを f32 に展開

    //u32 FloatCompression::F32ToF10(f32 value);    // f32 を11bitに圧縮
    //f32 FloatCompression::F10ToF32(u32  value);   // 11bitを f32 に展開

    //u32 FloatCompression::Vec3ToU32(const Vec3& value);   // Vec3 を[11,11,10]の32bitに圧縮
    //Vec3 FloatCompression::U32ToVec3(u32  value);          // [11,11,10]の32bitを Vec3 に展開
}// namespace ob