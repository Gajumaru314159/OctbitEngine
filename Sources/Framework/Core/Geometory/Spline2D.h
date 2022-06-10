﻿//***********************************************************
//! @file
//! @brief		スプラインカーブ2D
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector2.h>
#include <Framework/Core/Template/include.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  スプラインカーブ
    //@―---------------------------------------------------------------------------
    class Spline2D {
    public:

        using point_type = Vec2;                //!< ポイント型
        using points_type = vector<point_type>; //!< ポイントリスト型

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        Spline2D() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  ポイントリストから構築
        //@―---------------------------------------------------------------------------
        explicit Spline2D(const points_type&);


        //@―---------------------------------------------------------------------------
        //! @brief  ポイントリストから構築
        //@―---------------------------------------------------------------------------
        explicit Spline2D(const points_type&&);


        //@―---------------------------------------------------------------------------
        //! @brief  ポイント・サイズ
        //@―---------------------------------------------------------------------------
        s32 size()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  ポイントが存在するか
        //@―---------------------------------------------------------------------------
        bool empty()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  ポイントリストを空にする
        //@―---------------------------------------------------------------------------
        void clear();


        //@―---------------------------------------------------------------------------
        //! @brief  カーブの長さを計算
        //! 
        //! @param resolution いくつの直線で近似した長さを計算するか
        //@―---------------------------------------------------------------------------
        f32 length(s32 resolution = 100)const;


        //@―---------------------------------------------------------------------------
        //! @brief  [t0,t1]のカーブの長さを計算
        //! 
        //! @details t0,t1が[0,1]以外の場合は[0,1]にクランプされた値が計算されます。
        //! @param t0 開始位置[0,t1]
        //! @param t1 終了位置[t1,1]
        //! @param resolution いくつの直線で近似した長さを計算するか
        //@―---------------------------------------------------------------------------
        f32 length(f32 t0, f32 t1, s32 resolution = 100)const;


        //@―---------------------------------------------------------------------------
        //! @brief  座標を計算
        //! 
        //! @param t 時間[0,1]
        //@―---------------------------------------------------------------------------
        point_type position(f32 t)const;


        //@―---------------------------------------------------------------------------
        //! @brief  速度を計算
        //! 
        //! @param t 時間[0,1]
        //@―---------------------------------------------------------------------------
        point_type velocity(f32 t)const;


        //@―---------------------------------------------------------------------------
        //! @brief  加速度を計算
        //! 
        //! @param t 時間[0,1]
        //@―---------------------------------------------------------------------------
        point_type acceleration(f32 t)const;


        //@―---------------------------------------------------------------------------
        //! @brief  曲率を計算
        //! 
        //! @param t 時間[0,1]
        //@―---------------------------------------------------------------------------
        f32 curvature(f32 t)const;


    public:

        points_type points; //!< ポイントリスト
        bool        closed; //!< 閉じているか

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief  ポイントリストから構築
    //@―---------------------------------------------------------------------------
    inline Spline2D::Spline2D(const points_type& another) {
        points = another;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ポイントリストから構築
    //@―---------------------------------------------------------------------------
    inline Spline2D::Spline2D(const points_type&& another) {
        points = move(another);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ポイント・サイズ
    //@―---------------------------------------------------------------------------
    inline s32 Spline2D::size()const noexcept {
        return static_cast<s32>(points.size());
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ポイントが存在するか
    //@―---------------------------------------------------------------------------
    inline bool Spline2D::empty()const noexcept {
        return points.empty();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ポイントリストを空にする
    //@―---------------------------------------------------------------------------
    inline void Spline2D::clear() {
        points.clear();
    }

    //! @endcond
}// namespcae ob