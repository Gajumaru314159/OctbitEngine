//***********************************************************
//! @file
//! @brief		HSVカラー
//! @author		Gajumaru
//***********************************************************
#include "HSV.h"

namespace ob::core {

    const HSV HSV::white = { 0,0,1,1 };
    const HSV HSV::grey = { 0,0,0.5,1 };
    const HSV HSV::black = { 0,0,0,1 };
    const HSV HSV::clear = { 0,0,1,0 };
    const HSV HSV::red = { 0,1,1,1 };
    const HSV HSV::yellow = { 0.16667f,1,1,1 };
    const HSV HSV::green = { 0.33333f,1,1,1 };
    const HSV HSV::cyan = { 0.5f,1,1,1 };
    const HSV HSV::blue = { 0.66667f,1,1,1 };
    const HSV HSV::magenta = { 0.83333f,1,1,1 };


    //@―---------------------------------------------------------------------------
    //! @brief Vec3 を変換して初期化(アルファは1.0)
    //@―---------------------------------------------------------------------------
    HSV::HSV(const Vec3& another) noexcept {
        set(another.x, another.y, another.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief Vec4 を変換して初期化
    //@―---------------------------------------------------------------------------
    HSV::HSV(const Vec4& another) noexcept {
        set(another.x, another.y, another.z, another.w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief			RGBAカラーから設定
    //! 
    //! @param color    色
    //@―---------------------------------------------------------------------------
    void HSV::set(const Color& color) noexcept {
        f32 min = color.minComponent();
        f32 max = color.maxComponent();
        h = max - min;
        if (h > 0.0f) {
            if (max == color.r) {
                h = (color.g - color.b) / h;
                if (h < 0.0f) {
                    h += 6.0f;
                }
            } else if (max == color.g) {
                h = 2.0f + (color.b - color.r) / h;
            } else {
                h = 4.0f + (color.r - color.g) / h;
            }
        }
        h *= (1.0f / 6.0f);
        s = (max - min);
        if (max != 0.0f)s /= max;
        v = max;
        a = color.a;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      Color型に変換
    //@―---------------------------------------------------------------------------
    Color HSV::toColor()const {
        // 色相は循環可
        auto clamp_h = Math::Fract(1.0f + Math::Fract(h));

        f32 c = v * s;
        f32 hp = Math::Fract(clamp_h) * 6.0f;
        f32 x = c * (1.0f - Math::Abs(Math::Fract(hp * 0.5f) * 2.0f - 1.0f));
        f32 m = v - c;

        if (hp < 1.0f)return Color(c + m, x + m, 0 + m, a);
        else if (hp < 2.0f)return Color(x + m, c + m, 0 + m, a);
        else if (hp < 3.0f)return Color(0 + m, c + m, x + m, a);
        else if (hp < 4.0f)return Color(0 + m, x + m, c + m, a);
        else if (hp < 5.0f)return Color(x + m, 0 + m, c + m, a);
        else			   return Color(c + m, 0 + m, x + m, a);
    }

}// namespace ob