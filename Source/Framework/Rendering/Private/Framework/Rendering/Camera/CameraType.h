//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometory/Viewport.h>

namespace ob::rendering {
    

    //@―---------------------------------------------------------------------------
    //! @brief  カメラの投影モード
    //@―---------------------------------------------------------------------------
    enum class CameraType : u8{
        Perspective,    // 透視投影
        Orthographic,   // 平行投影
    };


    struct CameraState {
        Vec3        position;
        Rot         rotation;
        f32         fov;
        Viewport    viewport;
        f32         aspect;

    };





    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob