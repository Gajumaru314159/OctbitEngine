﻿//***********************************************************
//! @file
//! @brief		半直線
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector/Vector3.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  半直線
    //@―---------------------------------------------------------------------------
    class OB_API HalfLine {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        HalfLine() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(2点指定)
        //! 
        //! @param p0 始点
        //! @param p1 半直線状のもう1点
        //@―---------------------------------------------------------------------------
        HalfLine(const Vec3& p0, const Vec3& p1)noexcept;


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
    inline HalfLine::HalfLine(const Vec3& p0, const Vec3& p1)noexcept {
        set(p0, p1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(2点指定)
    //@―---------------------------------------------------------------------------
    inline void HalfLine::set(const Vec3& p0, const Vec3& p1)noexcept {
        this->p0 = p0;
        this->p1 = p1;
    }

    //! @endcond
}// namespcae ob