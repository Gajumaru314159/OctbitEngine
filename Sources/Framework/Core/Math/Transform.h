﻿//***********************************************************
//! @file
//! @brief		トランスフォーム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector3.h>
#include <Framework/Core/Math/Matrix.h>
#include <Framework/Core/Math/Rotation.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  トランスフォーム
    //@―---------------------------------------------------------------------------
    class Transform {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  Matrix に変換
        //@―---------------------------------------------------------------------------
        Matrix getMatrix()const;

    public:

        Vec3    position;   //!< 位置
        Rot     rotation;   //!< 回転
        Vec3    scale;      //!< 拡縮

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob