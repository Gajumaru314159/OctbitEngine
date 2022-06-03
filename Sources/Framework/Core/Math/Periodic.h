﻿//***********************************************************
//! @file
//! @brief		周期関数
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Math.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  周期関数
    //@―---------------------------------------------------------------------------
    class Periodic {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  サインカーブに従って周期的に[-1,1]の値を返す
        //@―---------------------------------------------------------------------------
        f32 Sin1_1(f32 period, f32 t)noexcept {
            return Math::Sin(t * Math::TWO_PI / period);
        }


        //@―---------------------------------------------------------------------------
        //! @brief  サインカーブに従って周期的に[0,1]の値を返す
        //@―---------------------------------------------------------------------------
        f32 Sin0_1(f32 period, f32 t)noexcept {
            return Sin1_1(period, t) * 0.5f + 0.5f;
        }


        //@―---------------------------------------------------------------------------
        //! @brief  矩形波に従って周期的に[-1,1]の値を返す
        //@―---------------------------------------------------------------------------
        f32 Square1_1(f32 period, f32 t)noexcept {
            return Square0_1(period, t) * 2.0f - 1.0f;
        }


        //@―---------------------------------------------------------------------------
        //! @brief  矩形波に従って周期的に[0,1]の値を返す
        //@―---------------------------------------------------------------------------
        f32 Square0_1(f32 period, f32 t)noexcept {
            return static_cast<s32>(t / period * 0.5f) % 2;
        }


        //@―---------------------------------------------------------------------------
        //! @brief  三角波に従って周期的に[-1,1]の値を返す
        //@―---------------------------------------------------------------------------
        f32 Triangle1_1(f32 period, f32 t)noexcept {
            return Triangle0_1(period, t) * 2.0f - 1.0f;
        }


        //@―---------------------------------------------------------------------------
        //! @brief  三角波に従って周期的に[0,1]の値を返す
        //@―---------------------------------------------------------------------------
        f32 Triangle0_1(f32 period, f32 t)noexcept {
            return 1.0f - Math::Abs(Math::Fract(t / period) * 2.0f - 1.0f);
        }


        //@―---------------------------------------------------------------------------
        //! @brief  鋸波に従って周期的に[-1,1]の値を返す
        //@―---------------------------------------------------------------------------
        f32 Sawthooth1_1(f32 period, f32 t)noexcept {
            return Sawthooth0_1(period, t) * 2.0f - 1.0f;
        }


        //@―---------------------------------------------------------------------------
        //! @brief  鋸波に従って周期的に[0,1]の値を返す
        //@―---------------------------------------------------------------------------
        f32 Sawthooth0_1(f32 period, f32 t)noexcept {
            return Math::Fract(t / period);
        }


    private:



    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob