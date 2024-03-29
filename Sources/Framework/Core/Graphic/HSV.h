﻿//***********************************************************
//! @file
//! @brief		HSVカラー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Mathf.h>
#include <Framework/Core/Math/Vector/include.h>
#include <Framework/Core/Graphic/Color.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief		HSVカラー
    //! 
    //! @details	各色要素は、0〜1の範囲のf32型で表現されます。アルファ(a)は透過度を表し、0で完全な透明、1で完全な不透明を表します。
    //@―---------------------------------------------------------------------------
    struct OB_API HSV {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        HSV()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(白で初期化)
        //! 
        //! @details		色を白(0,0,1,1)で初期化します。
        //@―---------------------------------------------------------------------------
        HSV(EForceInit)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(各要素を指定して初期化)
        //! 
        //! @details		各色成分を指定して初期化します。
        //! @param h		色相成分
        //! @param s		彩度成分
        //! @param v		明度成分
        //! @param a		アルファ
        //@―---------------------------------------------------------------------------
        HSV(f32 h, f32 s, f32 v, f32 a = 1.0f)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(Colorで初期化)
        //! 
        //! @param color    RGBA色
        //@―---------------------------------------------------------------------------
        HSV(const Color& color)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief Vec3 を変換して初期化(アルファは1.0)
        //@―---------------------------------------------------------------------------
        explicit HSV(const Vec3& another)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief Vec4 を変換して初期化
        //@―---------------------------------------------------------------------------
        explicit HSV(const Vec4& another)noexcept;


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 等価演算子
        //@―---------------------------------------------------------------------------
        bool operator == (const HSV& another) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子
        //@―---------------------------------------------------------------------------
        bool operator != (const HSV& another) const noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief		    色成分を指定
        //! 
        //! @param h		色相成分
        //! @param s		彩度成分
        //! @param v		明度成分
        //! @param a		アルファ
        //@―---------------------------------------------------------------------------
        void set(f32 h, f32 s, f32 v, f32 a = 1.0) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			RGBAカラーから設定
        //! 
        //! @param color    色
        //@―---------------------------------------------------------------------------
        void set(const Color& color) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      カラー要素を0.0～にクランプ
        //@―---------------------------------------------------------------------------
        void clamp() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      カラー要素を0.0～1.0にクランプ
        //@―---------------------------------------------------------------------------
        void clamp01() noexcept;


        //===============================================================
        //  変換
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      Color型に変換
        //@―---------------------------------------------------------------------------
        Color  toColor()const;


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      等価判定(許容誤差指定)
        //@―---------------------------------------------------------------------------
        bool equals(const HSV& another, f32 tolerance = Mathf::TOLERANCE)const noexcept;

    public:

        //@―---------------------------------------------------------------------------
        //! @brief		色の線形補完
        //! 
        //! @details	t が0のとき a を返し、 t が1のとき b を返す。
        //! @param a    色1
        //! @param b    色2
        //! @param t	補完パラメータ
        //! @return		補完された色オブジェクト
        //@―---------------------------------------------------------------------------
        static HSV Lerp(const HSV& a, const HSV& b, f32 t) noexcept;


    public:

        static const HSV white;		//!< HSV(0      ,0,1,1)
        static const HSV grey;		//!< HSV(0      ,0,0.5,1)
        static const HSV black;		//!< HSV(0      ,0,0,1)
        static const HSV clear;		//!< HSV(0      ,0,1,0)
        static const HSV red;		//!< HSV(0      ,1,1,1)
        static const HSV yellow;	//!< HSV(0.16667,1,1,1)
        static const HSV green;		//!< HSV(0.33333,1,1,1)
        static const HSV cyan;		//!< HSV(0.5    ,1,1,1)
        static const HSV blue;		//!< HSV(0.66667,1,1,1)
        static const HSV magenta;	//!< HSV(0.83333,1,1,1)

    public:

        f32 h;      //!< 色相成分
        f32 s;      //!< 彩度成分
        f32 v;      //!< 明度成分
        f32 a;      //!< アルファ成分

    };






    //=======================================================================
    // インライン
    //=======================================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief デフォルトコンストラクタ(初期化なし)
    //@―---------------------------------------------------------------------------
    inline HSV::HSV() noexcept {
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(白で初期化)
    //! 
    //! @details		色を白(1,1,1,1)で初期化します。
    //@―---------------------------------------------------------------------------
    inline HSV::HSV(EForceInit) noexcept {
        set(1, 1, 1, 1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(各要素を指定して初期化)
    //! 
    //! @details		各色成分を指定して初期化します。
    //! @param h		色相成分
    //! @param s		彩度成分
    //! @param v		明度成分
    //! @param a		アルファ
    //@―---------------------------------------------------------------------------
    inline HSV::HSV(f32 h, f32 s, f32 v, f32 a) noexcept {
        set(h, s, v, a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(Colorで初期化)
    //! 
    //! @param color    RGBA色
    //@―---------------------------------------------------------------------------
    inline HSV::HSV(const Color& color) noexcept {
        set(color);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    inline bool HSV::operator == (const HSV& another) const noexcept {
        return
            Mathf::IsNearEquals(h, another.h) &&
            Mathf::IsNearEquals(s, another.s) &&
            Mathf::IsNearEquals(v, another.v) &&
            Mathf::IsNearEquals(a, another.a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    inline bool HSV::operator != (const HSV& another) const noexcept {
        return !(operator==(another));
    }


    //@―---------------------------------------------------------------------------
    //! @brief		色成分を指定
    //! 
    //! @details	各色成分を指定して初期化します。
    //! @param h	色相成分
    //! @param s	彩度成分
    //! @param v	明度成分
    //! @param a	アルファ
    //@―---------------------------------------------------------------------------
    inline void HSV::set(f32 h, f32 s, f32 v, f32 a) noexcept {
        this->h = h;
        this->s = s;
        this->v = v;
        this->a = a;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      カラー要素を0.0～にクランプ
    //@―---------------------------------------------------------------------------
    inline void HSV::clamp() noexcept {
        h = Mathf::Max(h, 0.0f);
        s = Mathf::Max(s, 0.0f);
        v = Mathf::Max(v, 0.0f);
        a = Mathf::Clamp01(a);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      カラー要素を0.0～1.0にクランプ
    //@―---------------------------------------------------------------------------
    inline void HSV::clamp01() noexcept {
        h = Mathf::Clamp01(h);
        s = Mathf::Clamp01(s);
        v = Mathf::Clamp01(v);
        a = Mathf::Clamp01(a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      等価判定(許容誤差指定)
    //@―---------------------------------------------------------------------------
    inline bool HSV::equals(const HSV& another, f32 tolerance)const noexcept {
        return
            Mathf::IsNearEquals(h, another.h, tolerance) &&
            Mathf::IsNearEquals(s, another.s, tolerance) &&
            Mathf::IsNearEquals(v, another.v, tolerance) &&
            Mathf::IsNearEquals(a, another.a, tolerance);
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
    inline HSV HSV::Lerp(const HSV& a, const HSV& b, f32 t) noexcept {
        return HSV(
            Mathf::Lerp(a.h, b.h, t),
            Mathf::Lerp(a.s, b.s, t),
            Mathf::Lerp(a.v, b.v, t),
            Mathf::Lerp(a.a, b.a, t)
        );
    }

    //! @endcond
}// namespace ob