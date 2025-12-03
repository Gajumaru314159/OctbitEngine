//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/FloatCompression.h>
#include <Framework/Core/Math/Vector3.h>
#include <Framework/Core/Math/Math.h>
#include <algorithm>
#include <limits>



namespace ob::core {

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


    //! @brief f32 を 16bit小数に圧縮
    u32 FloatCompression::F32ToU16(f32 value) {
        u32 binary = std::bit_cast<u32>(value);

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

        // 16bit浮動小数点数のフォーマット: 符号ビット(1) | 指数部(5) | 仮数部(10)
        u16 result = (note & 0x8000) | ((exp << 10) & 0x7c00) | (decimal & 0x03ff);

        return result;
    }


    //! @brief 16bit小数を f32 に展開
    f32 FloatCompression::U16ToF32(u16 value) {
        u32 expNum = static_cast<u32>((value & 0x7c00) >> 10);

        if (expNum == 0)return 0.0f;

        f32 sign = static_cast<f32>((1 << 10) + (value & 0x03ff)) / static_cast<f32>(1 << 10);
        u32 note = (value & 0x8000) >> 15;
        f32 exp = s_float16ToFloat[note][expNum];

        return exp * sign;
    }

    //! @brief f32(-1.0f～1.0f) を u8 に圧縮
    u8 FloatCompression::F32ToU8(f32 value) {
        // -1.0f ～ 1.0f を 0 ～ 255 に変換
        value = std::clamp(value, -1.0f, 1.0f);
        return static_cast<u8>((value + 1.0f) * 127.5f);
    }

    //! @brief u8 を f32(-1.0f～1.0f) に展開
    f32 FloatCompression::U8ToF32(u8 value) {
        // 0 ～ 255 を -1.0f ～ 1.0f に変換
        return (static_cast<f32>(value) / 127.5f) - 1.0f;
    }

    //! @brief f32 を11bitに圧縮
    u32 FloatCompression::F32ToF11(f32 value) {
        // IEEE 754 32bit float を 11bit float に圧縮
        // Format: 符号なし指数(5bit) + 仮数部(6bit)
        u32 binary = std::bit_cast<u32>(value);
        
        if (value == 0.0f) return 0;
        
        // 符号ビットは無視（正の値のみサポート）
        u32 exp = (binary >> 23) & 0xFF;
        u32 mantissa = (binary >> 17) & 0x3F; // 上位6bitを取得
        
        if (exp == 0) return 0; // 非正規化数は0とする
        
        // 指数を11bit形式に変換 (バイアス調整: 127 -> 15)
        if (exp >= 127) {
            exp = exp - 127 + 15;
            if (exp >= 31) {
                exp = 31; // 無限大
                mantissa = 0;
            }
        } else {
            // 指数が小さすぎる場合は0
            return 0;
        }
        
        return (exp << 6) | mantissa;
    }

    //! @brief 11bitを f32 に展開
    f32 FloatCompression::F11ToF32(u32 value) {
        if (value == 0) return 0.0f;
        
        u32 exp = (value >> 6) & 0x1F;
        u32 mantissa = value & 0x3F;
        
        if (exp == 31) {
            // 無限大またはNaN
            return std::numeric_limits<f32>::infinity();
        }
        
        if (exp == 0) return 0.0f;
        
        // 32bit float形式に変換
        u32 exp32 = (exp - 15 + 127) << 23;
        u32 mantissa32 = mantissa << 17;
        
        u32 result = exp32 | mantissa32;
        return std::bit_cast<f32>(result);
    }

    //! @brief f32 を10bitに圧縮
    u32 FloatCompression::F32ToF10(f32 value) {
        // IEEE 754 32bit float を 10bit float に圧縮
        // Format: 符号なし指数(5bit) + 仮数部(5bit)
        u32 binary = std::bit_cast<u32>(value);
        
        if (value == 0.0f) return 0;
        
        u32 exp = (binary >> 23) & 0xFF;
        u32 mantissa = (binary >> 18) & 0x1F; // 上位5bitを取得
        
        if (exp == 0) return 0;
        
        // 指数を10bit形式に変換
        if (exp >= 127) {
            exp = exp - 127 + 15;
            if (exp >= 31) {
                exp = 31;
                mantissa = 0;
            }
        } else {
            return 0;
        }
        
        return (exp << 5) | mantissa;
    }

    //! @brief 10bitを f32 に展開
    f32 FloatCompression::F10ToF32(u32 value) {
        if (value == 0) return 0.0f;
        
        u32 exp = (value >> 5) & 0x1F;
        u32 mantissa = value & 0x1F;
        
        if (exp == 31) {
            return std::numeric_limits<f32>::infinity();
        }
        
        if (exp == 0) return 0.0f;
        
        // 32bit float形式に変換
        u32 exp32 = (exp - 15 + 127) << 23;
        u32 mantissa32 = mantissa << 18;
        
        u32 result = exp32 | mantissa32;
        return std::bit_cast<f32>(result);
    }

    //! @brief Vec3 を[11,11,10]の32bitに圧縮
    u32 FloatCompression::Vec3ToU32(const Vec3& value) {
        // X, Y: 11bit, Z: 10bit の合計32bitに圧縮
        u32 x = F32ToF11(value.x) & 0x7FF;  // 11bit
        u32 y = F32ToF11(value.y) & 0x7FF;  // 11bit  
        u32 z = F32ToF10(value.z) & 0x3FF;  // 10bit
        
        return (x << 21) | (y << 10) | z;
    }

    //! @brief [11,11,10]の32bitを Vec3 に展開
    Vec3 FloatCompression::U32ToVec3(u32 value) {
        u32 x = (value >> 21) & 0x7FF;  // 11bit
        u32 y = (value >> 10) & 0x7FF;  // 11bit
        u32 z = value & 0x3FF;          // 10bit
        
        return Vec3(
            F11ToF32(x),
            F11ToF32(y), 
            F10ToF32(z)
        );
    }
}