﻿//***********************************************************
//! @file
//! @brief		HSVカラー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Math.h>
#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Graphic/Color.h>

namespace ob::core {

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
        //! @brief          デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        HSV()noexcept =default;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(各要素を指定して初期化)
        //! 
        //! @details		各色成分を指定して初期化します。
        //! @param h		色相成分
        //! @param s		彩度成分
        //! @param v		明度成分
        //! @param a		アルファ
        //@―---------------------------------------------------------------------------
        constexpr HSV(f32 h, f32 s, f32 v, f32 a = 1.0f)noexcept;


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
        constexpr bool operator == (const HSV& another) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator != (const HSV& another) const noexcept;


        //===============================================================
        //  変換
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      Color型に変換
        //@―---------------------------------------------------------------------------
        Color  toColor()const;


        //===============================================================
        //  操作
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      カラー要素を0.0～にクランプ
        //@―---------------------------------------------------------------------------
        constexpr void clamp() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      カラー要素を0.0～1.0にクランプ
        //@―---------------------------------------------------------------------------
        constexpr void clamp01() noexcept;


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      等価判定(許容誤差指定)
        //@―---------------------------------------------------------------------------
        constexpr bool equals(const HSV& another, f32 tolerance = Math::TOLERANCE)const noexcept;

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
        static constexpr HSV Lerp(const HSV& a, const HSV& b, f32 t) noexcept;


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
    //! @brief コンストラクタ(各要素を指定して初期化)
    //! 
    //! @details		各色成分を指定して初期化します。
    //! @param h		色相成分
    //! @param s		彩度成分
    //! @param v		明度成分
    //! @param a		アルファ
    //@―---------------------------------------------------------------------------
    constexpr HSV::HSV(f32 h, f32 s, f32 v, f32 a) noexcept
        : h(h),s(s),v(v),a(a)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool HSV::operator == (const HSV& another) const noexcept {
        return
            Math::IsNearEquals(h, another.h) &&
            Math::IsNearEquals(s, another.s) &&
            Math::IsNearEquals(v, another.v) &&
            Math::IsNearEquals(a, another.a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool HSV::operator != (const HSV& another) const noexcept {
        return !(operator==(another));
    }


    //@―---------------------------------------------------------------------------
    //! @brief      カラー要素を0.0～にクランプ
    //@―---------------------------------------------------------------------------
    constexpr void HSV::clamp() noexcept {
        h = Math::Max(h, 0.0f);
        s = Math::Max(s, 0.0f);
        v = Math::Max(v, 0.0f);
        a = Math::Clamp01(a);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      カラー要素を0.0～1.0にクランプ
    //@―---------------------------------------------------------------------------
    constexpr void HSV::clamp01() noexcept {
        h = Math::Clamp01(h);
        s = Math::Clamp01(s);
        v = Math::Clamp01(v);
        a = Math::Clamp01(a);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      等価判定(許容誤差指定)
    //@―---------------------------------------------------------------------------
    constexpr bool HSV::equals(const HSV& another, f32 tolerance)const noexcept {
        return
            Math::IsNearEquals(h, another.h, tolerance) &&
            Math::IsNearEquals(s, another.s, tolerance) &&
            Math::IsNearEquals(v, another.v, tolerance) &&
            Math::IsNearEquals(a, another.a, tolerance);
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
    constexpr HSV HSV::Lerp(const HSV& a, const HSV& b, f32 t) noexcept {
        return HSV(
            Math::Lerp(a.h, b.h, t),
            Math::Lerp(a.s, b.s, t),
            Math::Lerp(a.v, b.v, t),
            Math::Lerp(a.a, b.a, t)
        );
    }

    //! @endcond
}// namespace ob


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::HSV, ob::core::Char> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template<typename FormatContext>
    auto format(ob::core::HSV value, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(), TC("({},{},{},{})"), value.h, value.s, value.v, value.a);
    }
};
//! @endcond