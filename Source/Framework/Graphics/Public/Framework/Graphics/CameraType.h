//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometry/Viewport.h>
#include <Framework/RHI/Forward.h>

#include <Framework/Graphics/RenderView.h>

namespace ob::graphics {
    

    //@―---------------------------------------------------------------------------
    //! @brief  カメラの投影モード
    //@―---------------------------------------------------------------------------
    enum class CameraType : u32{
        Perspective,    //!< 透視投影
        Orthographic,   //!< 平行投影
    };


    //@―---------------------------------------------------------------------------
    //! @brief  カメラの状態
    //@―---------------------------------------------------------------------------
    struct CameraState {
        Vec3        position;   //!< カメラのワールド座標
        Quat        rotation;   //!< カメラの回転
        Viewport    viewport;   //!< ビューポートサイズ
        f32         fov;        //!< 画角
    };


    class RenderView;

    struct CameraData {
        CameraType  type;
        Vec3        position;   //!< カメラのワールド座標
        Quat        rotation;   //!< カメラの回転
        f32         fov;        //!< 画角
        Viewport    viewport;   //!< ビューポートサイズ
        Ref<rhi::RenderTexture> renderTexture;
        UPtr<RenderView> renderView;

        Matrix      viewMatrix;
        Matrix      projMatrix;
        Matrix      viewprojMatrix;
    };

}