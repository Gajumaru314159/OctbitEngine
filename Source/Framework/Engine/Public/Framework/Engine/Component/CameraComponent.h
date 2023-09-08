//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/CameraType.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Texture.h>
#include <Framework/Engine/Component.h>

namespace ob::engine {

    //@―---------------------------------------------------------------------------
    //! @brief      カメラ
    //! @details    CameraCompoentとは異なりReflectionCaptureやShadowCaptureでも
    //!             使用されます。
    //@―---------------------------------------------------------------------------
    class CameraComponent : public Component{
    public:

        OB_RTTI();

        CameraComponent();

        // Field of View
        f32         getFov()const;
        void        setFov(f32 value);

        // クリップ範囲
        Range       getClipRange()const;
        void        setClipRange(Range range);

        // カメラ形式
        graphics::CameraType  getCameraType()const;
        void        setCameraType(graphics::CameraType value);

        // Viewport Rect
        Rect        getVieportRect();
        void        setVieportRect(Rect rect);

        // クリアカラー
        Color       getClearColor()const;
        void        setClearColor(const Color&);

        // 描画先(ディスプレイ)
        s32         getDisplay()const;
        void        setDisplay(s32 displayNo);

        // 描画先(RenderTexture)
        void        setRenderTarget(const Ref<rhi::RenderTexture>&);
        auto        getRenderTarget()const -> const Ref<rhi::RenderTexture>&;

        //LayerMask getLayerMask()const;
        //void setLayerMask(LayerMask mask);

        //auto getPriority()const->CameraPriority;
        //void setPriority(CameraPriority priority);

    public:

        void        setRenderView(UPtr<graphics::RenderView>);

    public:

        const Matrix& getViewMatrix()const;
        const Matrix& getProjectionMatrix()const;
        const Matrix& getViewProjectionMatrix()const;

        Vec3        worldToScreen(const Vec3& position)const;
        Vec3        screenToWorld(const Vec3& position,f32 distance)const;

    private:

        graphics::CameraType  m_type;
        Rect        m_rect;
        f32         m_fovY;
        Range       m_clipRange;

        //LayerMask   m_layerMask;
        //CameraPriority m_priority;
        Color       m_clearColor;
        s32         m_display;

        Ref<graphics::Camera> m_camera;
        Ref<rhi::RenderTexture> m_renderTexture;


    };

}