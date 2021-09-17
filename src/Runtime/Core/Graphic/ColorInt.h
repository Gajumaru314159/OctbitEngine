//***********************************************************
//! @file
//! @brief		整数表現カラークラス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Graphic/Color.h>

namespace ob {

    //-----------------------------------------------------------------------------
    //! @brief		整数表現カラークラス
    //! 
    //! @details	各色要素は、0〜255の範囲の u8 型で表現されます。アルファ(a)は透過度を表し、0で完全な透明、255で完全な不透明を表します。
    //-----------------------------------------------------------------------------
    struct OB_API ColorInt {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        ColorInt() noexcept;                                             // 白で初期化
        ColorInt(s32 r, s32 g, s32 b, s32 a = 255) noexcept;             // 各要素を指定して初期化
        ColorInt(s32 grey, s32 a = 255) noexcept;                        // 輝度とアルファ値を指定して初期化
        explicit ColorInt(const Color& another) noexcept;                // ColorU8 を変換して初期化
        explicit ColorInt(const Vec3& another) noexcept;                 // Vec3 を変換して初期化(アルファは1.0に設定)
        explicit ColorInt(const Vec4& another) noexcept;                 // Vec4 を変換して初期化


        //===============================================================
        //  オペレータ
        //===============================================================
        ColorInt operator + (const ColorInt& another) const noexcept;     // 加算
        ColorInt operator - (const ColorInt& another) const noexcept;     // 減算
        ColorInt& operator += (const ColorInt& another) noexcept;         // 加算代入
        ColorInt& operator -= (const ColorInt& another) noexcept;         // 減算代入


        //===============================================================
        //  セッター
        //===============================================================
        void Set(s32 r, s32 g, s32 b, s32 a = 255) noexcept;            // 色成分を指定
        void Set(s32 grey, s32 a = 255) noexcept;                       // 色成分を輝度で指定


        //===============================================================
        //  ゲッター
        //===============================================================
        u8 GetMin()const noexcept;                                      // RGBのうち最小の値を取得    
        u8 GetMax()const noexcept;                                      // RGBのうち最大の値を取得


        //===============================================================
        //  変換
        //===============================================================
        u32 ToCode(ColorCodeFormat format = ColorCodeFormat::DEFAULT)const noexcept;    //色をカラーコードに変換

    public:

        static const ColorInt black;		//!< Color(0,0,0,255)
        static const ColorInt blue;		//!< Color(0,0,255,255)
        static const ColorInt clear;		//!< Color(0,0,0,0)
        static const ColorInt cyan;		//!< Color(0,255,255,255)
        static const ColorInt green;		//!< Color(0,255,0,255)
        static const ColorInt grey;		//!< Color(0.5,0.5,0.5,255)
        static const ColorInt magenta;	//!< Color(255,0,255,255)
        static const ColorInt red;		//!< Color(255,0,0,255)
        static const ColorInt white;		//!< Color(255,255,255,255)
        static const ColorInt yellow;	//!< Color(255,235,4,255)

    public:

        union {
            struct {
                u8 r;       //!< 赤成分
                u8 g;       //!< 緑成分
                u8 b;       //!< 青成分
                u8 a;       //!< アルファ成分
            };
            u8 data[4];     //!< 色成分
            u32 code;       //!< カラーコード
        };

    };






    //=======================================================================
    // インライン
    //=======================================================================

    //-----------------------------------------------------------------------------
    //! @brief			コンストラクタ
    //! 
    //! @details		色を白(255,255,255,255)で初期化します。
    //-----------------------------------------------------------------------------
    inline ColorInt::ColorInt() noexcept {
        Set(1, 1, 1, 1);
    }


    //-----------------------------------------------------------------------------
    //! @brief			コンストラクタ
    //! 
    //! @details		各色成分を指定して初期化します。
    //! @param r		赤成分
    //! @param g		緑成分
    //! @param b		青成分
    //! @param a		アルファ
    //-----------------------------------------------------------------------------
    inline ColorInt::ColorInt(s32 r, s32 g, s32 b, s32 a) noexcept {
        Set(r, g, b, a);
    }


    //-----------------------------------------------------------------------------
    //! @brief			コンストラクタ
    //! @details		色成分を輝度で指定します。
    //! @param grey	輝度
    //! @param a		アルファ
    //-----------------------------------------------------------------------------
    inline ColorInt::ColorInt(s32 grey, s32 a) noexcept {
        Set(grey, grey, grey, a);
    }


    //-----------------------------------------------------------------------------
    //! @brief			加算演算子
    //-----------------------------------------------------------------------------
    inline ColorInt ColorInt::operator + (const ColorInt& another) const noexcept {
        return ColorInt(*this) += another;
    }


    //-----------------------------------------------------------------------------
    //! @brief			減算演算子
    //-----------------------------------------------------------------------------
    inline ColorInt ColorInt::operator - (const ColorInt& another) const noexcept {
        return ColorInt(*this) -= another;
    }


    //-----------------------------------------------------------------------------
    //! @brief			加算代入演算子
    //-----------------------------------------------------------------------------
    inline ColorInt& ColorInt::operator += (const ColorInt& another) noexcept {
        Set(r + another.r, g + another.g, b + another.b, a);
        return *this;
    }


    //-----------------------------------------------------------------------------
    //! @brief			減算代入演算子
    //-----------------------------------------------------------------------------
    inline ColorInt& ColorInt::operator -= (const ColorInt& another) noexcept {
        Set(r - another.r, g - another.g, b - another.b, a);
        return *this;
    }


    //-----------------------------------------------------------------------------
    //! @brief		色成分を指定
    //! 
    //! @param r	赤成分
    //! @param g	緑成分
    //! @param b	青成分
    //! @param a	アルファ
    //-----------------------------------------------------------------------------
    inline void ColorInt::Set(s32 r, s32 g, s32 b, s32 a) noexcept {
        this->r = (u8)get_clamp(r, 0, 255);
        this->g = (u8)get_clamp(g, 0, 255);
        this->b = (u8)get_clamp(b, 0, 255);
        this->a = (u8)get_clamp(a, 0, 255);
    }


    //-----------------------------------------------------------------------------
    //! @brief			色成分を輝度で指定
    //! 
    //! @param grey	輝度
    //! @param a		アルファ
    //-----------------------------------------------------------------------------
    inline void ColorInt::Set(s32 grey, s32 a) noexcept {
        Set(grey, grey, grey, a);
    }


    //-----------------------------------------------------------------------------
    //! @brief      RGBのうち最小の値を取得
    //-----------------------------------------------------------------------------
    inline u8 ColorInt::GetMin()const noexcept {
        return get_min(r, g, b);
    }


    //-----------------------------------------------------------------------------
    //! @brief      RGBのうち最大の値を取得
    //-----------------------------------------------------------------------------
    inline u8 ColorInt::GetMax()const noexcept {
        return get_max(r, g, b);
    }

}// namespcae ob