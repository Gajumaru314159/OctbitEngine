//***********************************************************
//! @file
//! @brief		カラー
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Graphic/Color.h>
#include <Framework/Core/Graphic/IntColor.h>

#include <Framework/Core/Math/Mathf.h>
#include <Framework/Core/Math/Vector/include.h>

namespace ob {

    const Color Color::blue = { 0,0,0,1 };
    const Color Color::clear = { 0,0,0,0 };
    const Color Color::cyan = { 0,1,1,1 };
    const Color Color::green = { 0,1,0,1 };
    const Color Color::grey = { 0.5,0.5,0.5,1 };
    const Color Color::magenta = { 1,0,1,1 };
    const Color Color::red = { 1,0,0,1 };
    const Color Color::white = { 1,1,1,1 };
    const Color Color::yellow = { 1,0.92f,0.016f,1 };

    const IntColor IntColor::blue = { 0,0,0,255 };
    const IntColor IntColor::clear = { 0,0,0,0 };
    const IntColor IntColor::cyan = { 0,255,255,255 };
    const IntColor IntColor::green = { 0,255,0,255 };
    const IntColor IntColor::grey = { 128,128,128,255 };
    const IntColor IntColor::magenta = { 255,0,255,255 };
    const IntColor IntColor::red = { 255,0,0,255 };
    const IntColor IntColor::white = { 255,255,255,255 };
    const IntColor IntColor::yellow = { 255,235,4,255 };


    //@―---------------------------------------------------------------------------
    //! @brief ColorU8 を変換して初期化
    //@―---------------------------------------------------------------------------
    Color::Color(const IntColor& another) noexcept {
        const f32 div = 1.0f / 255.0f;
        Set(another.r * div, another.g * div, another.b * div, another.a * div);
    }


    //@―---------------------------------------------------------------------------
    //! @brief Vec3 を変換して初期化(アルファは1.0)
    //@―---------------------------------------------------------------------------
    Color::Color(const Vec3& another) noexcept {
        Set(another.x, another.y, another.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief Vec4 を変換して初期化
    //@―---------------------------------------------------------------------------
    Color::Color(const Vec4& another) noexcept {
        Set(another.x, another.y, another.z, another.w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 色をカラーコードに変換
    //@―---------------------------------------------------------------------------
    u32 Color::ToCode(ColorCodeFormat format)const noexcept {
        IntColor c32=ToIntColor();
        return c32.ToCode();
    }


    //@―---------------------------------------------------------------------------
    //! @brief          IntColorに変換
    //@―---------------------------------------------------------------------------
    IntColor Color::ToIntColor()const noexcept {
        return IntColor(
            static_cast<s32>(get_clamp01(r) * 255),
            static_cast<s32>(get_clamp01(g) * 255),
            static_cast<s32>(get_clamp01(b) * 255),
            static_cast<s32>(get_clamp01(a) * 255));
    }


    //@―---------------------------------------------------------------------------
    //! @brief			RGBカラーをHSVに変換する
    //! 
    //! @param[out] h   色相出力先
    //! @param[out] s   彩度出力先
    //! @param[out] v   輝度出力先
    //@―---------------------------------------------------------------------------
    void Color::ToHSV(f32& h, f32& s, f32& v) noexcept {
        f32 max = Mathf::Max(r, g, b);
        f32 min = Mathf::Min(r, g, b);
        h = max - min;
        if (h > 0.0f) {
            if (max == r) {
                h = (g - b) / h;
                if (h < 0.0f) {
                    h += 6.0f;
                }
            } else if (max == g) {
                h = 2.0f + (b - r) / h;
            } else {
                h = 4.0f + (r - g) / h;
            }
        }
        h *= (1.0f / 6.0f);
        s = (max - min);
        if (max != 0.0f)s /= max;
        v = max;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			RGBカラーをHSVに変換する
    //! 
    //! @param[out] vec	出力先
    //@―---------------------------------------------------------------------------
    void Color::ToHSV(Vec3& vec) noexcept {
        ToHSV(vec.x, vec.y, vec.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      リニアカラーに変換
    //@―---------------------------------------------------------------------------
    Color Color::ToLinear()const {
        Color linear;
        auto convert = [](f32 value) {
            return value <= 0.04045f ? value / 12.92f : Mathf::Pow(value + 0.055f, 2.4f);
        };

        linear.r = convert(r);
        linear.g = convert(g);
        linear.b = convert(b);
        linear.a = a;
        return linear;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      sRGBカラーに変換
    //@―---------------------------------------------------------------------------
    Color Color::ToSRGB()const {
        Color linear;
        auto convert = [](f32 value) {
            return value <= 0.0031308f ? value * 12.92f : Mathf::Pow(value, 1.0f / 2.4f) * 1.055f - 0.055f;
        };

        linear.r = convert(r);
        linear.g = convert(g);
        linear.b = convert(b);
        linear.a = a;
        return linear;
    }


    //@―---------------------------------------------------------------------------
    //! @brief		HSV色空間からRGB色を生成する
    //! @param h	色相(0～1)
    //! @param s	彩度(0～1)
    //! @param v	輝度(0～1)
    //! @param a	アルファ(0～1)
    //! @return		色オブジェクト
    //@―---------------------------------------------------------------------------
    Color Color::HSV(f32 h, f32 s, f32 v, f32 a) noexcept {
        // 色相は循環可
        h = Mathf::Fract(1.0f + Mathf::Fract(h));

        f32 c = v * s;
        f32 hp = Mathf::Fract(h) * 6.0f;
        f32 x = c * (1.0f - Mathf::Abs(Mathf::Fract(hp * 0.5f) * 2.0f - 1.0f));
        f32 m = v - c;

        if (hp < 1.0f)return Color(c + m, x + m, 0 + m, a);
        else if (hp < 2.0f)return Color(x + m, c + m, 0 + m, a);
        else if (hp < 3.0f)return Color(0 + m, c + m, x + m, a);
        else if (hp < 4.0f)return Color(0 + m, x + m, c + m, a);
        else if (hp < 5.0f)return Color(x + m, 0 + m, c + m, a);
        else			   return Color(c + m, 0 + m, x + m, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief		色の線形補完
    //! 
    //! @details	t が0のとき col1 を返し、 t が1のとき col2 を返す。
    //! @param a    色1
    //! @param b    色2
    //! @param t	補完パラメータ
    //! @return		補完された色オブジェクト
    //@―---------------------------------------------------------------------------
    Color Color::Lerp(Color a, Color b, f32 t) noexcept {
        return (a * t) + (b * (1.0f - t));
    }


    //@―---------------------------------------------------------------------------
    //! @brief		HSV空間で色の線形補完
    //! 
    //! @details	t が0のとき col1 を返し、 t が1のとき col2 を返す。
    //! @param a    色1
    //! @param b    色2
    //! @param t	補完パラメータ
    //! @return		補完された色オブジェクト
    //@―---------------------------------------------------------------------------
    Color Color::LerpHSV(Color a, Color b, f32 t) noexcept {
        f32 aH, aS, aV;
        f32 bH, bS, bV;
        a.ToHSV(aH, aS, aV);
        b.ToHSV(bH, bS, bV);

        if (0.5f < Mathf::Abs(aH - bH)) {
            if (aH < bH)
                aH += 1.0f;
            else
                bH += 1.0f;
        }

        return Color(Mathf::Fract(Mathf::Lerp(aH, bH, t)), Mathf::Lerp(aS, bS, t), Mathf::Lerp(aV, bV, t));
    }

}// namespace ob