﻿//***********************************************************
//! @file
//! @brief		視錐台
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief      視錐台
    //@―---------------------------------------------------------------------------
    struct OB_API Frustum {
        f32 left;	//!< 左座標
        f32 right;	//!< 右座標
        f32 bottom;	//!< 下座標
        f32 top;	//!< 上座標
        f32 zNear;	//!< ニアクリッピング面
        f32 zFar;	//!< ファークリッピング面
    };





    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob