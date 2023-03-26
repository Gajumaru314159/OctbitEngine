﻿//***********************************************************
//! @file
//! @brief		線分
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector3.h>


namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  線分
    //@―---------------------------------------------------------------------------
    class OB_API Segment {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Segment() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(2点指定)
        //@―---------------------------------------------------------------------------
        constexpr Segment(Vec3 p0, Vec3 p1)noexcept;

    public:

        Vec3 p0;    //!< 始点
        Vec3 p1;    //!< 終点

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(2点指定)
    //@―---------------------------------------------------------------------------
    constexpr Segment::Segment(Vec3 p0, Vec3 p1)noexcept
        : p0(p0),p1(p1)
    {
    }

    //! @endcond
}// namespcae ob