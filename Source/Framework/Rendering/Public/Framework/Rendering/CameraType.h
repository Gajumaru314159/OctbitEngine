//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometory/Viewport.h>

namespace ob::graphic {
    

    //@―---------------------------------------------------------------------------
    //! @brief  カメラの投影モード
    //@―---------------------------------------------------------------------------
    enum class CameraProjectionMode : u32{
        Perspective,    // 透視投影
        Orthographic,   // 平行投影
    };


    struct CameraState {
        Vec3    position;
        Rot     rotation;
        f32     fov;
        Viewport viewport;
        f32     aspect;

    };

    class Camera {
    public:

    private:
        CameraProjectionMode m_projectionMode;
        f32 m_fov;
        Range m_clipRange;

        Rect m_viewportRect;

    };





    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob