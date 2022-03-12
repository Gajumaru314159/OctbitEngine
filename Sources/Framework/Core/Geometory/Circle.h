﻿//***********************************************************
//! @file
//! @brief		円
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector/Vector2.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  円
    //@―---------------------------------------------------------------------------
    struct OB_API Circle {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Circle() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ( ゼロ初期化 )
        //@―---------------------------------------------------------------------------
        Circle(EForceInit)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Circle(const Vec2& center, f32 radius)noexcept :center(center), radius(radius) {};


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          等価演算子
        //@―---------------------------------------------------------------------------
        bool operator==(const Circle& other)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          否等価演算子
        //@―---------------------------------------------------------------------------
        bool operator!=(const Circle& other)const noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          中心座標と半径を設定
        //! 
        //! @param center   中心座標
        //! @param radius   半径
        //@―---------------------------------------------------------------------------
        void set(const Vec2& center, f32 radius)noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          面積を取得
        //@―---------------------------------------------------------------------------
        f32 area()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          円周の長さを取得
        //@―---------------------------------------------------------------------------
        f32 perimeter()const noexcept;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      補間
        //! 
        //! @param a    開始
        //! @param b    終了
        //! @param t    補間係数
        //! @return     t=0のときa、t=1の時bを返す。
        //@―---------------------------------------------------------------------------
        static Circle Lerp(const Circle& a, const Circle& b, f32 t)noexcept;


    public:

        Vec2    center; //!< 中心座標
        f32     radius; //!< 半径

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ( ゼロ初期化 )
    //@―---------------------------------------------------------------------------
    inline Circle::Circle(EForceInit)noexcept {
        center.setZero();
        radius = 0.0f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Circle::operator==(const Circle& other)const noexcept {
        return center == other.center && radius == other.radius;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          否等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Circle::operator!=(const Circle& other)const noexcept {
        return !(*this == other);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          中心座標と半径を設定
    //! 
    //! @param center   中心座標
    //! @param radius   半径
    //@―---------------------------------------------------------------------------
    inline void Circle::set(const Vec2& center, f32 radius)noexcept {
        this->center = center;
        this->radius = radius;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          面積を取得
    //@―---------------------------------------------------------------------------
    inline f32 Circle::area()const noexcept {
        return radius * radius * Mathf::PI;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          円周の長さを取得
    //@―---------------------------------------------------------------------------
    inline f32 Circle::perimeter()const noexcept {
        return 2.0f * Mathf::PI;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      補間
    //! 
    //! @param a    開始
    //! @param b    終了
    //! @param t    補間係数
    //! @return     t=0のときa、t=1の時bを返す。
    //@―---------------------------------------------------------------------------
    inline Circle Circle::Lerp(const Circle& a, const Circle& b, f32 t)noexcept {
        return Circle(Vec2::Lerp(a.center,b.center,t),Mathf::Lerp(a.radius,b.radius,t));
    }

    //! @endcond
}// namespcae ob