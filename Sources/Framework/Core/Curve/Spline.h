﻿//***********************************************************
//! @file
//! @brief		スプラインカーブ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  スプラインカーブ
    //@―---------------------------------------------------------------------------
    class Spline {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  エルミート・カーブ
        //@―---------------------------------------------------------------------------
        template<class T>
        static constexpr T Hermite(const T& p0, const T& p1, const T& m0, const T& m1, f32 t)noexcept;

        //@―---------------------------------------------------------------------------
        //! @brief  カットマル・カーブ
        //@―---------------------------------------------------------------------------
        template<class T>
        static constexpr T Catmull(const T& p0, const T& p1, const T& p2, const T& p3, f32 t)noexcept;


    private:

        //@―---------------------------------------------------------------------------
        //! @brief  接線計算
        //@―---------------------------------------------------------------------------
        template<class T>
        static constexpr T CalculateTangent(const T& p0, const T& p1, const T& p2,f32 a,f32 b)noexcept;


    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief  エルミート・カーブ
    //@―---------------------------------------------------------------------------
    template <class T>
    inline constexpr T Spline::Hermite(const T& p0, const T& p1, const T& m0, const T& m1, f32 t) noexcept {
        return (2 * (t * t * t) - 3 * (t * t) + 1) * p0 + ((t * t * t) - 2 * (t * t) + t) * m0 + ((t * t * t) - (t * t)) * m1 + (-2 * (t * t * t) + 3 * (t * t)) * p1;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  カットマル・カーブ
    //@―---------------------------------------------------------------------------
    template <class T>
    inline constexpr T Spline::Catmull(const T& p0, const T& p1, const T& p2, const T& p3, f32 t) noexcept {
        return Hermite(p1, p2, CalculateTangent(p0, p1, p2), CalculateTangent(p1, p2, p3), t);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  接線計算
    //@―---------------------------------------------------------------------------
    template<class T>
    inline constexpr T Spline::CalculateTangent(const T& p0, const T& p1, const T& p2, f32 a, f32 b) noexcept {
        return ((1 - a) * (1 + b)) / 2 * (p1 - p0) + ((1 - a) * (1 - b)) / 2 * (p2 - p1);
    }

    //! @endcond
}// namespcae ob