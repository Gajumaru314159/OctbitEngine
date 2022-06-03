﻿//***********************************************************
//! @file
//! @brief		整数表現カラークラス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Graphic/ColorTypes.h>
#include <Framework/Core/Template/include.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief		整数表現カラークラス
    //! 
    //! @details	各色要素は、0〜255の範囲の u8 型で表現されます。アルファ(a)は透過度を表し、0で完全な透明、255で完全な不透明を表します。
    //@―---------------------------------------------------------------------------
    struct OB_API IntColor {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief			デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        IntColor() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			コンストラクタ(白で初期化)
        //! 
        //! @details		色を白(255,255,255,255)で初期化します。
        //@―---------------------------------------------------------------------------
        IntColor(EForceInit) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			コンストラクタ(各要素を指定して初期化)
        //! 
        //! @details		各色成分を指定して初期化します。
        //! @param r		赤成分
        //! @param g		緑成分
        //! @param b		青成分
        //! @param a		アルファ
        //@―---------------------------------------------------------------------------
        IntColor(s32 r, s32 g, s32 b, s32 a = 255) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ(輝度とアルファ値を指定して初期化)
        //! 
        //! @details		色成分を輝度で指定します。
        //! @param grey	    輝度
        //! @param a		アルファ
        //@―---------------------------------------------------------------------------
        IntColor(s32 grey, s32 a = 255) noexcept;


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief			加算演算子
        //@―---------------------------------------------------------------------------
        IntColor operator + (const IntColor& another) const noexcept;

        //@―---------------------------------------------------------------------------
        //! @brief			減算演算子
        //@―---------------------------------------------------------------------------
        IntColor operator - (const IntColor& another) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			加算代入演算子
        //@―---------------------------------------------------------------------------
        IntColor& operator += (const IntColor& another) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			減算代入演算子
        //@―---------------------------------------------------------------------------
        IntColor& operator -= (const IntColor& another) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子
        //@―---------------------------------------------------------------------------
        IntColor operator * (const IntColor& another) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          乗算代入演算子
        //@―---------------------------------------------------------------------------
        IntColor& operator *= (const IntColor& another) noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief		    色成分を指定
        //! 
        //! @param r	    赤成分
        //! @param g	    緑成分
        //! @param b	    青成分
        //! @param a	    アルファ
        //@―---------------------------------------------------------------------------
        void set(s32 r, s32 g, s32 b, s32 a = 255) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief		    色成分を輝度で指定
        //! 
        //! @param grey     輝度
        //! @param a	    アルファ
        //@―---------------------------------------------------------------------------
        void set(s32 grey, s32 a = 255) noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          RGBのうち最小の値を取得
        //@―---------------------------------------------------------------------------
        u8 minComponent()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          RGBのうち最大の値を取得
        //@―---------------------------------------------------------------------------
        u8 maxComponent()const noexcept;


        //===============================================================
        //  変換
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          色をカラーコードに変換
        //! 
        //! @param          format  カラーフォーマット(色の並び)
        //@―---------------------------------------------------------------------------
        u32 toCode(ColorCodeFormat format = ColorCodeFormat::Default)const noexcept;

    public:

        static const IntColor white;		//!< Color(255,255,255,255)
        static const IntColor grey;		    //!< Color(128.128.128,255)
        static const IntColor black;		//!< Color(0  ,0  ,0  ,255)
        static const IntColor clear;		//!< Color(0  ,0  ,0  ,0  )
        static const IntColor red;		    //!< Color(255,0  ,0  ,255)
        static const IntColor yellow;	    //!< Color(255,255,0  ,255)
        static const IntColor green;		//!< Color(0  ,255,0  ,255)
        static const IntColor cyan;		    //!< Color(0  ,255,255,255)
        static const IntColor blue;		    //!< Color(0  ,0  ,255,255)
        static const IntColor magenta;	    //!< Color(255,0  ,255,255)

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
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief			コンストラクタ
    //! 
    //! @details		色を白(255,255,255,255)で初期化します。
    //@―---------------------------------------------------------------------------
    inline IntColor::IntColor() noexcept {
    }


    //@―---------------------------------------------------------------------------
    //! @brief			コンストラクタ
    //! 
    //! @details		色を白(255,255,255,255)で初期化します。
    //@―---------------------------------------------------------------------------
    inline IntColor::IntColor(EForceInit) noexcept {
        set(1, 1, 1, 1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief			コンストラクタ
    //! 
    //! @details		各色成分を指定して初期化します。
    //! @param r		赤成分
    //! @param g		緑成分
    //! @param b		青成分
    //! @param a		アルファ
    //@―---------------------------------------------------------------------------
    inline IntColor::IntColor(s32 r, s32 g, s32 b, s32 a) noexcept {
        set(r, g, b, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief			コンストラクタ
    //! 
    //! @details		色成分を輝度で指定します。
    //! @param grey	輝度
    //! @param a		アルファ
    //@―---------------------------------------------------------------------------
    inline IntColor::IntColor(s32 grey, s32 a) noexcept {
        set(grey, grey, grey, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief			加算演算子
    //@―---------------------------------------------------------------------------
    inline IntColor IntColor::operator + (const IntColor& another) const noexcept {
        return IntColor(*this) += another;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			減算演算子
    //@―---------------------------------------------------------------------------
    inline IntColor IntColor::operator - (const IntColor& another) const noexcept {
        return IntColor(*this) -= another;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			乗算演算子
    //@―---------------------------------------------------------------------------
    inline IntColor IntColor::operator * (const IntColor& another) const noexcept {
        return IntColor(*this) *= another;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			加算代入演算子
    //@―---------------------------------------------------------------------------
    inline IntColor& IntColor::operator += (const IntColor& another) noexcept {
        set(r + another.r, g + another.g, b + another.b, a);
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			減算代入演算子
    //@―---------------------------------------------------------------------------
    inline IntColor& IntColor::operator -= (const IntColor& another) noexcept {
        set(r - another.r, g - another.g, b - another.b, a);
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			乗算代入演算子
    //@―---------------------------------------------------------------------------
    inline IntColor& IntColor::operator *= (const IntColor& another) noexcept {
        set((s32)r * another.r / 255,
            (s32)g * another.g / 255,
            (s32)b * another.b / 255,
            (s32)a * another.a / 255);
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
    inline void IntColor::set(s32 r, s32 g, s32 b, s32 a) noexcept {
        this->r = (u8)std::clamp(r, 0, 255);
        this->g = (u8)std::clamp(g, 0, 255);
        this->b = (u8)std::clamp(b, 0, 255);
        this->a = (u8)std::clamp(a, 0, 255);
    }


    //@―---------------------------------------------------------------------------
    //! @brief			色成分を輝度で指定
    //! 
    //! @param grey	輝度
    //! @param a		アルファ
    //@―---------------------------------------------------------------------------
    inline void IntColor::set(s32 grey, s32 a) noexcept {
        set(grey, grey, grey, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      RGBのうち最小の値を取得
    //@―---------------------------------------------------------------------------
    inline u8 IntColor::minComponent()const noexcept {
        return std::min({ r, g, b });
    }


    //@―---------------------------------------------------------------------------
    //! @brief      RGBのうち最大の値を取得
    //@―---------------------------------------------------------------------------
    inline u8 IntColor::maxComponent()const noexcept {
        return std::min({ r, g, b });
    }

    //! @endcond
}// namespcae ob