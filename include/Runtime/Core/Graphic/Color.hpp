//***********************************************************
//! @file
//! @brief		カラー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

#include <Core/Math/Mathf.hpp>
#include <Core/Math/Vector/include.hpp>

namespace ob
{

    //-----------------------------------------------------------
    //! @brief		色空間列挙型
    //-----------------------------------------------------------
    enum class GammaSpace :u8
    {
        Liner,      //!< 線形
        Pow22,      //!<
        sRGB        //!< ガンマ
    };


    //-----------------------------------------------------------
    //! @brief		色空間列挙型
    //-----------------------------------------------------------
    enum class ColorCodeFormat:u8
    {
        ARGB,
        ABGR,
        RGBA,
        BGRA,
        DEFAULT = ARGB
    };

    struct Color32;

    //-----------------------------------------------------------
    //! @brief		RGBAカラーを表現するクラス
    //! @details	各色要素は、0〜1の範囲のf32型で表現されます。アルファ(a)は透過度を表し、0で完全な透明、1で完全な不透明を表します。
    //-----------------------------------------------------------
    struct OB_API Color
    {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        Color();                                            // 白で初期化
        Color(f32 r_, f32 g_, f32 b_, f32 a_ = 1.0);        // 各要素を指定して初期化
        Color(f32 grey_, f32 a_ = 1.0);                     // 輝度とアルファ値を指定して初期化
        explicit Color(const Color32& another);             // ColorU8 を変換して初期化
        explicit Color(const Vec3& another);                // Vec3 を変換して初期化(アルファは1.0に設定)
        explicit Color(const Vec4& another);                // Vec4 を変換して初期化


        //===============================================================
        //  オペレータ
        //===============================================================
        bool operator == (const Color& another) const;      // 等価演算子
        bool operator != (const Color& another) const;      // 否等価演算子
        Color operator + (const Color& another) const;      // 加算
        Color operator - (const Color& another) const;      // 減算
        Color operator * (const Color& another) const;      // 乗算
        Color operator * (f32 f) const;                     // 乗算(スカラー)
        Color operator / (const Color& another) const;      // 乗算
        Color operator / (f32 f) const;                     // 乗算(スカラー)
        Color& operator += (const Color& another);          // 加算代入
        Color& operator -= (const Color& another);          // 減算代入
        Color& operator *= (Color& another);                // 乗算代入
        Color& operator *= (f32 f);                         // 乗算代入(スカラー)
        Color& operator /= (Color& another);                // 乗算代入
        Color& operator /= (f32 f);                         // 乗算代入(スカラー)


        //===============================================================
        //  セッター
        //===============================================================
        void Set(f32 r_, f32 g_, f32 b_, f32 a_ = 1.0);     // 色成分を指定
        void Set(f32 grey_, f32 a_ = 1.0);                  // 色成分を輝度で指定
        void Clamp();                                       // カラー要素を0.0～にクランプ
        void Clamp01();                                     // カラー要素を0.0～1.0にクランプ


        //===============================================================
        //  ゲッター
        //===============================================================
        f32 GetMin()const;                                  // RGBのうち最小の値を取得
        f32 GetMax()const;                                  // RGBのうち最大の値を取得

        //===============================================================
        //  変換
        //===============================================================
        u32  ToCode(ColorCodeFormat format = ColorCodeFormat::DEFAULT)const;   // 色をカラーコードに変換
        void ToHSV(f32& h, f32& s, f32& v);                 // RGBカラーをHSVに変換
        void ToHSV(Vec3& vec);                              // RGBカラーをHSVに変換
        Color ToGrayscale()const;                           // グレイスケールに変換
        Color ToSepia()const;                               // セピアカラーに変換


        //===============================================================
        //  判定
        //===============================================================
        b32 Equals(const Color& another, f32 tolerance = Mathf::TOLERANCE)const;    // 等価判定(許容範囲指定)


    public:

        static Color HSV(f32 h, f32 s, f32 v, f32 a);       // HSV色空間からRGB色を生成
        static Color Lerp(Color a, Color b, f32 t);         // 色の線形補完
        static Color LerpHSV(Color a, Color b, f32 t);      // HSV空間で色の線形補完

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

        union
        {
            struct
            {
                f32 r;//!< 赤成分
                f32 g;//!< 緑成分
                f32 b;//!< 青成分
                f32 a;//!< アルファ成分
            };
            f32 data[4];    //!< ベクトルの成分
        };

    };






    //=======================================================================
    // インライン
    //=======================================================================

    //-----------------------------------------------------------
    //! @brief コンストラクタ(白で初期化)
    //-----------------------------------------------------------
    inline Color::Color()
    {
        Set(1, 1, 1, 1);
    }


    //-----------------------------------------------------------
    //! @brief コンストラクタ(各要素を指定して初期化)
    //-----------------------------------------------------------
    inline Color::Color(f32 r_, f32 g_, f32 b_, f32 a_)
    {
        Set(r_, g_, b_, a_);
    }


    //-----------------------------------------------------------
    //! @brief コンストラクタ(輝度とアルファ値を指定して初期化)
    //-----------------------------------------------------------
    inline Color::Color(f32 grey_, f32 a_)
    {
        Set(grey_, grey_, grey_, a);
    }


    //-----------------------------------------------------------
    //! @brief 等価演算子
    //-----------------------------------------------------------
    inline bool Color::operator == (const Color& another) const
    {
        return
            Mathf::Approximately(r, another.r) &&
            Mathf::Approximately(g, another.g) &&
            Mathf::Approximately(b, another.b) &&
            Mathf::Approximately(a, another.a);
    }


    //-----------------------------------------------------------
    //! @brief 否等価演算子
    //-----------------------------------------------------------
    inline bool Color::operator != (const Color& another) const
    {
        return !(operator==(another));
    }


    //-----------------------------------------------------------
    //! @brief 加算演算子
    //-----------------------------------------------------------
    inline Color Color::operator + (const Color& another) const
    {
        return Color(*this) += another;
    }


    //-----------------------------------------------------------
    //! @brief 減算演算子
    //-----------------------------------------------------------
    inline Color Color::operator - (const Color& another) const
    {
        return Color(*this) -= another;
    }


    //-----------------------------------------------------------
    //! @brief 乗算演算子(スカラー)
    //-----------------------------------------------------------
    inline Color Color::operator * (f32 f) const
    {
        return Color(*this) *= f;
    }


    //-----------------------------------------------------------
    //! @brief 乗算演算子
    //-----------------------------------------------------------
    inline Color Color::operator * (const Color& another) const
    {
        return Color(*this) * another;
    }


    //-----------------------------------------------------------
    //! @brief 除算演算子(スカラー)
    //-----------------------------------------------------------
    inline Color Color::operator / (f32 f) const
    {
        return Color(*this) /= f;
    }


    //-----------------------------------------------------------
    //! @brief 除算演算子
    //-----------------------------------------------------------
    inline Color Color::operator / (const Color& another) const
    {
        return Color(*this) / another;
    }


    //-----------------------------------------------------------
    //! @brief 加算代入演算子
    //-----------------------------------------------------------
    inline Color& Color::operator += (const Color& another)
    {
        Set(r + another.r, g + another.g, b + another.b, a+another.a);
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 減算代入演算子
    //-----------------------------------------------------------
    inline Color& Color::operator -= (const Color& another)
    {
        Set(r - another.r, g - another.g, b - another.b, a-another.a);
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 乗算代入演算子(スカラー)
    //-----------------------------------------------------------
    inline Color& Color::operator *= (f32 f)
    {
        r *= f;
        g *= f;
        b *= f;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 乗算代入演算子
    //-----------------------------------------------------------
    inline Color& Color::operator *= (Color& another)
    {
        r *= another.r;
        g *= another.g;
        b *= another.b;
        a *= another.a;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 除算代入演算子(スカラー)
    //-----------------------------------------------------------
    inline Color& Color::operator /= (f32 f)
    {
        r /= f;
        g /= f;
        b /= f;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 除算代入演算子
    //-----------------------------------------------------------
    inline Color& Color::operator /= (Color& another)
    {
        r /= another.r;
        g /= another.g;
        b /= another.b;
        a /= another.a;
        return *this;
    }


    //-----------------------------------------------------------
    // @brief		色成分を指定
    // 
    // @param r_	赤成分
    // @param g_	緑成分
    // @param b_	青成分
    // @param a_	アルファ
    //-----------------------------------------------------------
    inline void Color::Set(f32 r_, f32 g_, f32 b_, f32 a_)
    {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }


    //-----------------------------------------------------------
    // @brief			色成分を輝度で指定
    // 
    // @param grey_	輝度
    // @param a_		アルファ
    //-----------------------------------------------------------
    inline void Color::Set(f32 grey_, f32 a_)
    {
        r = grey_;
        g = grey_;
        b = grey_;
        a = a_;
    }


    //-----------------------------------------------------------
    // @brief      カラー要素を0.0～にクランプ
    //-----------------------------------------------------------
    inline void Color::Clamp()
    {
        r = Mathf::Max(r, 0.0f);
        g = Mathf::Max(g, 0.0f);
        b = Mathf::Max(b, 0.0f);
        a = Mathf::Clamp01(a);
    }

    //-----------------------------------------------------------
    // @brief      カラー要素を0.0～1.0にクランプ
    //-----------------------------------------------------------
    inline void Color::Clamp01()
    {
        r = Mathf::Clamp01(r);
        g = Mathf::Clamp01(g);
        b = Mathf::Clamp01(b);
        a = Mathf::Clamp01(a);
    }


    //-----------------------------------------------------------
    // @brief      RGBのうち最小の値を取得
    //-----------------------------------------------------------
    inline f32 Color::GetMin()const
    {
        return Mathf::Min(r,g,b);
    }


    //-----------------------------------------------------------
    // @brief      RGBのうち最大の値を取得
    //-----------------------------------------------------------
    inline f32 Color::GetMax()const
    {
        return Mathf::Max(r,g,b);
    }


    //-----------------------------------------------------------
    // @brief      グレイスケールに変換
    //-----------------------------------------------------------
    inline Color Color::ToGrayscale()const
    {
        f32 gray = r * 0.29f + g * 0.57f + b * 0.14f;
        return Color(gray, a);
    }


    //-----------------------------------------------------------
    // @brief      セピアカラーに変換
    //-----------------------------------------------------------
    inline Color Color::ToSepia()const
    {
        Color sepia = this->ToGrayscale();
        sepia.r *= 1.351f;
        sepia.g *= 1.2f;
        sepia.b *= 0.934f;
        sepia.Clamp01();
        return sepia;
    }


    //-----------------------------------------------------------
    //! @brief      等価判定(許容範囲指定)
    //-----------------------------------------------------------
    inline b32 Color::Equals(const Color& another, f32 tolerance)const
    {
        return
            Mathf::Approximately(r, another.r,tolerance) &&
            Mathf::Approximately(g, another.g,tolerance) &&
            Mathf::Approximately(b, another.b,tolerance) &&
            Mathf::Approximately(a, another.a,tolerance);
    }

}// namespace ob