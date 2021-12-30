﻿//***********************************************************
//! @file
//! @brief		カラー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Core/Math/Mathf.h>
#include <Runtime/Core/Math/Vector/include.h>

namespace ob {
    
    //@―---------------------------------------------------------------------------
    //! @brief		色空間列挙型
    //@―---------------------------------------------------------------------------
    enum class GammaSpace :u8 {
        Liner,      //!< 線形
        Pow22,      //!<
        sRGB        //!< ガンマ
    };


    //@―---------------------------------------------------------------------------
    //! @brief		色空間列挙型
    //@―---------------------------------------------------------------------------
    enum class ColorCodeFormat :u8 {
        ARGB,
        ABGR,
        RGBA,
        BGRA,
        DEFAULT = ARGB
    };

    struct IntColor;

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
        void Set(f32 r, f32 g, f32 b, f32 a = 1.0) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			色成分を輝度で指定
        //! 
        //! @param grey	    輝度
        //! @param a		アルファ
        //@―---------------------------------------------------------------------------
        void Set(f32 grey, f32 a = 1.0) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      カラー要素を0.0～にクランプ
        //@―---------------------------------------------------------------------------
        void Clamp() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      カラー要素を0.0～1.0にクランプ
        //@―---------------------------------------------------------------------------
        void Clamp01() noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================
        
        //@―---------------------------------------------------------------------------
        //! @brief      RGBのうち最小の値を取得
        //@―---------------------------------------------------------------------------
        f32 GetMinComponent()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      RGBのうち最大の値を取得
        //@―---------------------------------------------------------------------------
        f32 GetMaxComponent()const noexcept; 


        //===============================================================
        //  変換
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          色をカラーコードに変換
        //@―---------------------------------------------------------------------------
        u32  ToCode(ColorCodeFormat format = ColorCodeFormat::DEFAULT)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			RGBカラーをHSVに変換する
        //! 
        //! @param[out] h   色相出力先
        //! @param[out] s   彩度出力先
        //! @param[out] v   輝度出力先
        //@―---------------------------------------------------------------------------
        void ToHSV(f32& h, f32& s, f32& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			RGBカラーをHSVに変換する
        //! 
        //! @param[out] vec	出力先
        //@―---------------------------------------------------------------------------
        void ToHSV(Vec3& vec) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief           グレイスケールに変換
        //@―---------------------------------------------------------------------------
        Color ToGrayscale()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          セピアカラーに変換
        //@―---------------------------------------------------------------------------
        Color ToSepia()const noexcept;


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      等価判定(許容誤差指定)
        //@―---------------------------------------------------------------------------
        bool Equals(const Color& another, f32 tolerance = Mathf::TOLERANCE)const noexcept;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief		HSV色空間からRGB色を生成する
        //! @param h	色相(0～1)
        //! @param s	彩度(0～1)
        //! @param v	輝度(0～1)
        //! @param a	アルファ(0～1)
        //! @return		色オブジェクト
        //@―---------------------------------------------------------------------------
        static Color HSV(f32 h, f32 s, f32 v, f32 a) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief		色の線形補完
        //! 
        //! @details	t が0のとき col1 を返し、 t が1のとき col2 を返す。
        //! @param a    色1
        //! @param b    色2
        //! @param t	補完パラメータ
        //! @return		補完された色オブジェクト
        //@―---------------------------------------------------------------------------
        static Color Lerp(Color a, Color b, f32 t) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief		HSV空間で色の線形補完
        //! 
        //! @details	t が0のとき col1 を返し、 t が1のとき col2 を返す。
        //! @param a    色1
        //! @param b    色2
        //! @param t	補完パラメータ
        //! @return		補完された色オブジェクト
        //@―---------------------------------------------------------------------------
        static Color LerpHSV(Color a, Color b, f32 t) noexcept;


    public:

        static const Color black;		//!< Color(0,0,0,1)
        static const Color blue;		//!< Color(0,0,1,1)
        static const Color clear;		//!< Color(0,0,0,0)
        static const Color cyan;		//!< Color(0,1,1,1)
        static const Color green;		//!< Color(0,1,0,1)
        static const Color grey;		//!< Color(0.5,0.5,0.5,1)
        static const Color magenta;		//!< Color(1,0,1,1)
        static const Color red;			//!< Color(1,0,0,1)
        static const Color white;		//!< Color(1,1,1,1)
        static const Color yellow;		//!< Color(1,0.92,0.016,1)

    public:

        union {
            struct {
                f32 r;      //!< 赤成分
                f32 g;      //!< 緑成分
                f32 b;      //!< 青成分
                f32 a;      //!< アルファ成分
            };
            f32 data[4];    //!< ベクトルの成分
        };

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
        Set(1, 1, 1, 1);
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
        Set(r, g, b, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(輝度とアルファ値を指定して初期化)
    //! 
    //! @details		色成分を輝度で指定します。
    //! @param grey	    輝度
    //! @param a		アルファ
    //@―---------------------------------------------------------------------------
    inline Color::Color(f32 grey, f32 a) noexcept {
        Set(grey, grey, grey, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Color::operator == (const Color& another) const noexcept {
        return
            Mathf::Approximately(r, another.r) &&
            Mathf::Approximately(g, another.g) &&
            Mathf::Approximately(b, another.b) &&
            Mathf::Approximately(a, another.a);
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
        Set(r + another.r, g + another.g, b + another.b, a + another.a);
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子
    //@―---------------------------------------------------------------------------
    inline Color& Color::operator -= (const Color& another) noexcept {
        Set(r - another.r, g - another.g, b - another.b, a - another.a);
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
    inline void Color::Set(f32 r, f32 g, f32 b, f32 a) noexcept {
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
    inline void Color::Set(f32 grey, f32 a) noexcept {
        r = grey;
        g = grey;
        b = grey;
        this->a = a;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      カラー要素を0.0～にクランプ
    //@―---------------------------------------------------------------------------
    inline void Color::Clamp() noexcept {
        r = Mathf::Max(r, 0.0f);
        g = Mathf::Max(g, 0.0f);
        b = Mathf::Max(b, 0.0f);
        a = Mathf::Clamp01(a);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      カラー要素を0.0～1.0にクランプ
    //@―---------------------------------------------------------------------------
    inline void Color::Clamp01() noexcept {
        r = Mathf::Clamp01(r);
        g = Mathf::Clamp01(g);
        b = Mathf::Clamp01(b);
        a = Mathf::Clamp01(a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      RGBのうち最小の値を取得
    //@―---------------------------------------------------------------------------
    inline f32 Color::GetMinComponent()const noexcept {
        return Mathf::Min(r, g, b);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      RGBのうち最大の値を取得
    //@―---------------------------------------------------------------------------
    inline f32 Color::GetMaxComponent()const noexcept {
        return Mathf::Max(r, g, b);
    }


    //@―---------------------------------------------------------------------------
    // @brief      グレイスケールに変換
    //@―---------------------------------------------------------------------------
    inline Color Color::ToGrayscale()const noexcept {
        f32 gray = r * 0.29f + g * 0.57f + b * 0.14f;
        return Color(gray, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      セピアカラーに変換
    //@―---------------------------------------------------------------------------
    inline Color Color::ToSepia()const noexcept {
        Color sepia = this->ToGrayscale();
        sepia.r *= 1.351f;
        sepia.g *= 1.2f;
        sepia.b *= 0.934f;
        sepia.Clamp01();
        return sepia;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      等価判定(許容誤差指定)
    //@―---------------------------------------------------------------------------
    inline bool Color::Equals(const Color& another, f32 tolerance)const noexcept {
        return
            Mathf::Approximately(r, another.r, tolerance) &&
            Mathf::Approximately(g, another.g, tolerance) &&
            Mathf::Approximately(b, another.b, tolerance) &&
            Mathf::Approximately(a, another.a, tolerance);
    }

    //! @endcond
}// namespace ob