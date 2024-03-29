﻿//***********************************************************
//! @file
//! @brief		スプラインカーブ2D
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Curve/Spline2D.h>

namespace ob
{


    //@―---------------------------------------------------------------------------
    //! @brief  カーブの長さを計算
    //! 
    //! @params resolution いくつの直線で近似した長さを計算するか
    //@―---------------------------------------------------------------------------
    f32 Spline2D::length(s32 resolution)const {
        OB_NOTIMPLEMENTED();
        return 0.0f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  [t0,t1]のカーブの長さを計算
    //! 
    //! @details t0,t1が[0,1]以外の場合は[0,1]にクランプされた値が計算されます。
    //! @params t0 開始位置[0,t1]
    //! @params t1 終了位置[t1,1]
    //! @params resolution いくつの直線で近似した長さを計算するか
    //@―---------------------------------------------------------------------------
    f32 Spline2D::length(f32 t0, f32 t1, s32 resolution)const {
        OB_NOTIMPLEMENTED();
        return 0.0f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  座標を計算
    //! 
    //! @params t 時間[0,1]
    //@―---------------------------------------------------------------------------
    Spline2D::point_type Spline2D::position(f32 t)const {
        OB_NOTIMPLEMENTED();
        return point_type();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  速度を計算
    //! 
    //! @params t 時間[0,1]
    //@―---------------------------------------------------------------------------
    Spline2D::point_type Spline2D::velocity(f32 t)const {
        OB_NOTIMPLEMENTED();
        return point_type();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  加速度を計算
    //! 
    //! @params t 時間[0,1]
    //@―---------------------------------------------------------------------------
    Spline2D::point_type Spline2D::acceleration(f32 t)const {
        OB_NOTIMPLEMENTED();
        return point_type();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  曲率を計算
    //! 
    //! @params t 時間[0,1]
    //@―---------------------------------------------------------------------------
    f32 Spline2D::curvature(f32 t)const {
        OB_NOTIMPLEMENTED();
        return 0.0f;
    }

}// namespace ob