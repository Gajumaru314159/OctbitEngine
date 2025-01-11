//***********************************************************
//! @file
//! @brief		スプラインカーブ2D
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometry/Spline2D.h>
#include <Framework/Core/Log/Assertion.h>

namespace ob::core {


    //! @brief  カーブの長さを計算
    //! 
    //! @params resolution いくつの直線で近似した長さを計算するか
    f32 Spline2D::length([[maybe_unused]]s32 resolution)const {
        OB_NOTIMPLEMENTED();
        return 0.0f;
    }


    //! @brief  [t0,t1]のカーブの長さを計算
    //! 
    //! @details t0,t1が[0,1]以外の場合は[0,1]にクランプされた値が計算されます。
    //! @params t0 開始位置[0,t1]
    //! @params t1 終了位置[t1,1]
    //! @params resolution いくつの直線で近似した長さを計算するか
    f32 Spline2D::length([[maybe_unused]] f32 t0, [[maybe_unused]] f32 t1, [[maybe_unused]] s32 resolution)const {
        OB_NOTIMPLEMENTED();
        return 0.0f;
    }


    //! @brief  座標を計算
    //! 
    //! @params t 時間[0,1]
    Vec2 Spline2D::position([[maybe_unused]] f32 t)const {
        OB_NOTIMPLEMENTED();
        return Vec2();
    }


    //! @brief  速度を計算
    //! 
    //! @params t 時間[0,1]
    Vec2 Spline2D::velocity([[maybe_unused]] f32 t)const {
        OB_NOTIMPLEMENTED();
        return Vec2();
    }


    //! @brief  加速度を計算
    //! 
    //! @params t 時間[0,1]
    Vec2 Spline2D::acceleration([[maybe_unused]] f32 t)const {
        OB_NOTIMPLEMENTED();
        return Vec2();
    }


    //! @brief  曲率を計算
    //! 
    //! @params t 時間[0,1]
    f32 Spline2D::curvature([[maybe_unused]] f32 t)const {
        OB_NOTIMPLEMENTED();
        return 0.0f;
    }

}