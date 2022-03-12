﻿//***********************************************************
//! @file
//! @brief		ライン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector/include.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  線分
    //@―---------------------------------------------------------------------------
    struct OB_API Line {
    public:


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Line() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(2点指定)
        //! 
        //! @param p0 直線上の点1
        //! @param p1 直線上の点2
        //@―---------------------------------------------------------------------------
        Line(const Vec3& p0, const Vec3& p1)noexcept;


        //===============================================================
        // セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  設定
        //! 
        //! @param p0 始点
        //! @param p1 半直線状のもう1点
        //@―---------------------------------------------------------------------------
        void set(const Vec3& p0, const Vec3& p1)noexcept;

    public:

        Vec3 p0;    //!< 始点
        Vec3 p1;    //!< 半直線状のもう1点

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(2点指定)
    //@―---------------------------------------------------------------------------
    inline Line::Line(const Vec3& p0, const Vec3& p1)noexcept {
        set(p0, p1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(2点指定)
    //! 
    //! @param p0 直線上の点1
    //! @param p1 直線上の点2
    //@―---------------------------------------------------------------------------
    inline void Line::set(const Vec3& p0, const Vec3& p1)noexcept {
        this->p0 = p0;
        this->p1 = p1;
    }


    //! @endcond
}// namespcae ob