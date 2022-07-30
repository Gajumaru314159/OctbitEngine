//***********************************************************
//! @file
//! @brief		カラー
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Graphic/Color.h>
#include <Framework/Core/Graphic/HSV.h>
#include <Framework/Core/Graphic/IntColor.h>

#include <Framework/Core/Math/Math.h>
#include <Framework/Core/Math/Vectors.h>

namespace ob::core {

    const Color Color::White = { 1,1,1,1 };
    const Color Color::Gray = { 0.5,0.5,0.5,1 };
    const Color Color::Black = { 0,0,0,1 };
    const Color Color::Clear = { 0,0,0,0 };
    const Color Color::Red = { 1,0,0,1 };
    const Color Color::Yellow = { 1,1,0,1 };
    const Color Color::Green = { 0,1,0,1 };
    const Color Color::Cyan = { 0,1,1,1 };
    const Color Color::Blue = { 0,0,1,1 };
    const Color Color::Magenta = { 1,0,1,1 };
    const Color Color::Normal = { 0.5f,1.0f,0.5f,1 };


    //@―---------------------------------------------------------------------------
    //! @brief ColorU8 を変換して初期化
    //@―---------------------------------------------------------------------------
    Color::Color(const IntColor& another) noexcept {
        const f32 div = 1.0f / 255.0f;
        set(another.r * div, another.g * div, another.b * div, another.a * div);
    }


    //@―---------------------------------------------------------------------------
    //! @brief Vec3 を変換して初期化(アルファは1.0)
    //@―---------------------------------------------------------------------------
    Color::Color(const Vec3& another) noexcept {
        set(another.x, another.y, another.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief Vec4 を変換して初期化
    //@―---------------------------------------------------------------------------
    Color::Color(const Vec4& another) noexcept {
        set(another.x, another.y, another.z, another.w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 色をカラーコードに変換
    //@―---------------------------------------------------------------------------
    u32 Color::toCode(ColorCodeFormat format)const noexcept {
        IntColor c32 = toIntColor();
        return c32.toCode();
    }


    //@―---------------------------------------------------------------------------
    //! @brief          IntColorに変換
    //@―---------------------------------------------------------------------------
    IntColor Color::toIntColor()const noexcept {
        return IntColor(
            static_cast<s32>(ob::core::clamp01(r) * 255),
            static_cast<s32>(ob::core::clamp01(g) * 255),
            static_cast<s32>(ob::core::clamp01(b) * 255),
            static_cast<s32>(ob::core::clamp01(a) * 255));
    }


    //@―---------------------------------------------------------------------------
    //! @brief      リニアカラーに変換
    //@―---------------------------------------------------------------------------
    Color Color::toLinear()const {
        Color linear;
        auto convert = [](f32 value) {
            return value <= 0.04045f ? value / 12.92f : Math::Pow(value + 0.055f, 2.4f);
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
    Color Color::toSRGB()const {
        Color linear;
        auto convert = [](f32 value) {
            return value <= 0.0031308f ? value * 12.92f : Math::Pow(value, 1.0f / 2.4f) * 1.055f - 0.055f;
        };

        linear.r = convert(r);
        linear.g = convert(g);
        linear.b = convert(b);
        linear.a = a;
        return linear;
    }


    //@―---------------------------------------------------------------------------
    //! @brief		色の線形補完
    //! 
    //! @details	t が0のとき a を返し、 t が1のとき b を返す。
    //! @param a    色1
    //! @param b    色2
    //! @param t	補完パラメータ
    //! @return		補完された色オブジェクト
    //@―---------------------------------------------------------------------------
    Color Color::Lerp(const Color& a, const Color& b, f32 t) noexcept {
        return (a * t) + (b * (1.0f - t));
    }


    //@―---------------------------------------------------------------------------
    //! @brief		HSV空間で色の線形補完
    //! 
    //! @details	t が0のとき a を返し、 t が1のとき b を返す。
    //! @param a    色1
    //! @param b    色2
    //! @param t	補完パラメータ
    //! @return		補完された色オブジェクト
    //@―---------------------------------------------------------------------------
    Color Color::LerpHSV(const Color& a, const Color& b, f32 t) noexcept {
        return HSV::Lerp(HSV(a), HSV(b), t).toColor();
    }


    //@―---------------------------------------------------------------------------
    //! @brief		カラーコードから色を生成する
    //! 
    //! @return		色オブジェクト
    //@―---------------------------------------------------------------------------
    Color Color::FromCode(StringView code) noexcept {
        auto size = code.size();
        if (size <= 1)return Color::White;
        if (code[0] == TC('#'))return Color::White;

        return Color::White;
    }

}// namespace ob