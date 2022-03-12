﻿//***********************************************************
//! @file
//! @brief		カラー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Mathf.h>
#include <Framework/Core/Math/Vector/include.h>
#include <Framework/Core/Graphic/ColorTypes.h>
#include <Framework/Core/Graphic/IntColor.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief		RGBAカラーを表現するクラス
    //! 
    //! @details	各色要素は、0〜1の範囲のf32型で表現されます。アルファ(a)は透過度を表し、0で完全な透明、1で完全な不透明を表します。
    //@―---------------------------------------------------------------------------
    struct OB_API Color {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Color()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(白で初期化)
        //! 
        //! @details		色を白(1,1,1,1)で初期化します。
        //@―---------------------------------------------------------------------------
        Color(EForceInit)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(各要素を指定して初期化)
        //! 
        //! @details		各色成分を指定して初期化します。
        //! @param r		赤成分
        //! @param g		緑成分
        //! @param b		青成分
        //! @param a		アルファ
        //@―---------------------------------------------------------------------------
        Color(f32 r, f32 g, f32 b, f32 a = 1.0f)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(輝度とアルファ値を指定して初期化)
        //! 
        //! @details		色成分を輝度で指定します。
        //! @param grey	    輝度
        //! @param a		アルファ
        //@―---------------------------------------------------------------------------
        Color(f32 grey, f32 a = 1.0f)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief ColorU8 を変換して初期化
        //@―---------------------------------------------------------------------------
        explicit Color(const IntColor& another)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief Vec3 を変換して初期化(アルファは1.0)
        //@―---------------------------------------------------------------------------
        explicit Color(const Vec3& another)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief Vec4 を変換して初期化
        //@―---------------------------------------------------------------------------
        explicit Color(const Vec4& another)noexcept;


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 等価演算子
        //@―---------------------------------------------------------------------------
        bool operator == (const Color& another) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子
        //@―---------------------------------------------------------------------------
        bool operator != (const Color& another) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子
        //@―---------------------------------------------------------------------------
        Color operator + (const Color& another) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子
        //@―---------------------------------------------------------------------------
        Color operator - (const Color& another) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子
        //@―---------------------------------------------------------------------------
        Color operator * (const Color& another) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Color operator * (f32 f) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子
        //@―---------------------------------------------------------------------------
        Color operator / (const Color& another) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Color operator / (f32 f) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子
        //@―---------------------------------------------------------------------------
        Color& operator += (const Color& another) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子
        //@―---------------------------------------------------------------------------
        Color& operator -= (const Color& another) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子
        //@―---------------------------------------------------------------------------
        Color& operator *= (Color& another) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Color& operator *= (f32 f) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子
        //@―---------------------------------------------------------------------------
        Color& operator /= (Color& another) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Color& operator /= (f32 f) noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief		色成分を指定
        //! 
        //! @param r	赤成分
        //! @param g	緑成分
        //! @param b	青成分
        //! @param a	アルファ
        //@―---------------------------------------------------------------------------
        void set(f32 r, f32 g, f32 b, f32 a = 1.0) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			色成分を輝度で指定
        //! 
        //! @param grey	    輝度
        //! @param a		アルファ
        //@―---------------------------------------------------------------------------
        void set(f32 grey, f32 a = 1.0) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      カラー要素を0.0～にクランプ
        //@―---------------------------------------------------------------------------
        void clamp() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      カラー要素を0.0～1.0にクランプ
        //@―---------------------------------------------------------------------------
        void clamp01() noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      RGBのうち最小の値を取得
        //@―---------------------------------------------------------------------------
        f32 minComponent()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      RGBのうち最大の値を取得
        //@―---------------------------------------------------------------------------
        f32 maxComponent()const noexcept;


        //===============================================================
        //  変換
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          色をカラーコードに変換
        //@―---------------------------------------------------------------------------
        u32  toCode(ColorCodeFormat format = ColorCodeFormat::Default)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          IntColor に変換
        //@―---------------------------------------------------------------------------
        IntColor toIntColor()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          Vec4 に変換
        //@―---------------------------------------------------------------------------
        Vec4 toVec4()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief           グレイスケールに変換
        //@―---------------------------------------------------------------------------
        Color toGrayscale()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          セピアカラーに変換
        //@―---------------------------------------------------------------------------
        Color toSepia()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          sRGBカラーをリニアカラーに変換
        //@―---------------------------------------------------------------------------
        Color toLinear()const;


        //@―---------------------------------------------------------------------------
        //! @brief          リニアカラーをsRGBカラーに変換
        //@―---------------------------------------------------------------------------
        Color toSRGB()const;


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      等価判定(許容誤差指定)
        //@―---------------------------------------------------------------------------
        bool equals(const Color& another, f32 tolerance = Mathf::TOLERANCE)const noexcept;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief		カラーコードから色を生成する
        //! @return		色オブジェクト
        //@―---------------------------------------------------------------------------
        static Color FromCode(StringView code) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief		色の線形補完
        //! 
        //! @details	t が0のとき a を返し、 t が1のとき b を返す。
        //! @param a    色1
        //! @param b    色2
        //! @param t	補完パラメータ
        //! @return		補完された色オブジェクト
        //@―---------------------------------------------------------------------------
        static Color Lerp(const Color& a, const Color& b, f32 t) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief		HSV空間で色の線形補完
        //! 
        //! @details	t が0のとき a を返し、 t が1のとき b を返す。
        //! @param a    色1
        //! @param b    色2
        //! @param t	補完パラメータ
        //! @return		補完された色オブジェクト
        //@―---------------------------------------------------------------------------
        static Color LerpHSV(const Color& a, const Color& b, f32 t) noexcept;


    public:

        static const Color white;		//!< Color(1,1,1,1)
        static const Color grey;		//!< Color(0.5,0.5,0.5,1)
        static const Color black;		//!< Color(0,0,0,1)
        static const Color clear;		//!< Color(0,0,0,0)
        static const Color red;			//!< Color(1,0,0,1)
        static const Color yellow;		//!< Color(1,1,0,1)
        static const Color green;		//!< Color(0,1,0,1)
        static const Color cyan;		//!< Color(0,1,1,1)
        static const Color blue;		//!< Color(0,0,1,1)
        static const Color magenta;		//!< Color(1,0,1,1)

    public:

        f32 r;      //!< 赤成分
        f32 g;      //!< 緑成分
        f32 b;      //!< 青成分
        f32 a;      //!< アルファ成分

    };






    //=======================================================================
    // インライン
    //=======================================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief デフォルトコンストラクタ(初期化なし)
    //@―---------------------------------------------------------------------------
    inline Color::Color() noexcept {
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(白で初期化)
    //! 
    //! @details		色を白(1,1,1,1)で初期化します。
    //@―---------------------------------------------------------------------------
    inline Color::Color(EForceInit) noexcept {
        set(1, 1, 1, 1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(各要素を指定して初期化)
    //! 
    //! @details		各色成分を指定して初期化します。
    //! @param r		赤成分
    //! @param g		緑成分
    //! @param b		青成分
    //! @param a		アルファ
    //@―---------------------------------------------------------------------------
    inline Color::Color(f32 r, f32 g, f32 b, f32 a) noexcept {
        set(r, g, b, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(輝度とアルファ値を指定して初期化)
    //! 
    //! @details		色成分を輝度で指定します。
    //! @param grey	    輝度
    //! @param a		アルファ
    //@―---------------------------------------------------------------------------
    inline Color::Color(f32 grey, f32 a) noexcept {
        set(grey, grey, grey, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Color::operator == (const Color& another) const noexcept {
        return
            Mathf::IsNearEquals(r, another.r) &&
            Mathf::IsNearEquals(g, another.g) &&
            Mathf::IsNearEquals(b, another.b) &&
            Mathf::IsNearEquals(a, another.a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Color::operator != (const Color& another) const noexcept {
        return !(operator==(another));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子
    //@―---------------------------------------------------------------------------
    inline Color Color::operator + (const Color& another) const noexcept {
        return Color(*this) += another;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子
    //@―---------------------------------------------------------------------------
    inline Color Color::operator - (const Color& another) const noexcept {
        return Color(*this) -= another;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline Color Color::operator * (f32 f) const noexcept {
        return Color(*this) *= f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子
    //@―---------------------------------------------------------------------------
    inline Color Color::operator * (const Color& another) const noexcept {
        return Color(*this) * another;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline Color Color::operator / (f32 f) const noexcept {
        return Color(*this) /= f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子
    //@―---------------------------------------------------------------------------
    inline Color Color::operator / (const Color& another) const noexcept {
        return Color(*this) / another;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子
    //@―---------------------------------------------------------------------------
    inline Color& Color::operator += (const Color& another) noexcept {
        set(r + another.r, g + another.g, b + another.b, a + another.a);
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子
    //@―---------------------------------------------------------------------------
    inline Color& Color::operator -= (const Color& another) noexcept {
        set(r - another.r, g - another.g, b - another.b, a - another.a);
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline Color& Color::operator *= (f32 f) noexcept {
        r *= f;
        g *= f;
        b *= f;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子
    //@―---------------------------------------------------------------------------
    inline Color& Color::operator *= (Color& another) noexcept {
        r *= another.r;
        g *= another.g;
        b *= another.b;
        a *= another.a;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline Color& Color::operator /= (f32 f) noexcept {
        r /= f;
        g /= f;
        b /= f;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子
    //@―---------------------------------------------------------------------------
    inline Color& Color::operator /= (Color& another) noexcept {
        r /= another.r;
        g /= another.g;
        b /= another.b;
        a /= another.a;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief		色成分を指定
    //! 
    //! @param r	赤成分
    //! @param g	緑成分
    //! @param b	青成分
    //! @param a	アルファ
    //@―---------------------------------------------------------------------------
    inline void Color::set(f32 r, f32 g, f32 b, f32 a) noexcept {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			色成分を輝度で指定
    //! 
    //! @param grey	    輝度
    //! @param a		アルファ
    //@―---------------------------------------------------------------------------
    inline void Color::set(f32 grey, f32 a) noexcept {
        r = grey;
        g = grey;
        b = grey;
        this->a = a;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      カラー要素を0.0～にクランプ
    //@―---------------------------------------------------------------------------
    inline void Color::clamp() noexcept {
        r = Mathf::Max(r, 0.0f);
        g = Mathf::Max(g, 0.0f);
        b = Mathf::Max(b, 0.0f);
        a = Mathf::Clamp01(a);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      カラー要素を0.0～1.0にクランプ
    //@―---------------------------------------------------------------------------
    inline void Color::clamp01() noexcept {
        r = Mathf::Clamp01(r);
        g = Mathf::Clamp01(g);
        b = Mathf::Clamp01(b);
        a = Mathf::Clamp01(a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      RGBのうち最小の値を取得
    //@―---------------------------------------------------------------------------
    inline f32 Color::minComponent()const noexcept {
        return Mathf::Min(r, g, b);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      RGBのうち最大の値を取得
    //@―---------------------------------------------------------------------------
    inline f32 Color::maxComponent()const noexcept {
        return Mathf::Max(r, g, b);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          Vec4 に変換
    //@―---------------------------------------------------------------------------
    inline Vec4 Color::toVec4()const noexcept {
        return Vec4(r, g, b, a);
    }


    //@―---------------------------------------------------------------------------
    // @brief      グレイスケールに変換
    //@―---------------------------------------------------------------------------
    inline Color Color::toGrayscale()const noexcept {
        f32 gray = r * 0.29f + g * 0.57f + b * 0.14f;
        return Color(gray, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      セピアカラーに変換
    //@―---------------------------------------------------------------------------
    inline Color Color::toSepia()const noexcept {
        Color sepia = this->toGrayscale();
        sepia.r *= 1.351f;
        sepia.g *= 1.2f;
        sepia.b *= 0.934f;
        sepia.clamp01();
        return sepia;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      等価判定(許容誤差指定)
    //@―---------------------------------------------------------------------------
    inline bool Color::equals(const Color& another, f32 tolerance)const noexcept {
        return
            Mathf::IsNearEquals(r, another.r, tolerance) &&
            Mathf::IsNearEquals(g, another.g, tolerance) &&
            Mathf::IsNearEquals(b, another.b, tolerance) &&
            Mathf::IsNearEquals(a, another.a, tolerance);
    }

    //! @endcond
}// namespace ob