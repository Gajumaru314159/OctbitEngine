//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Camera.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      カメラ
    //! @details    CameraCompoentとは異なりReflectionCaptureやShadowCaptureでも
    //!             使用されます。
    //@―---------------------------------------------------------------------------
    class CameraImpl :public Camera {
    public:

        CameraImpl(UPtr<RenderView>);
        ~CameraImpl();

        //===============================================================
        // プロパティ
        //===============================================================
        f32 getFov()const override;
        void setFov(f32 value) override;

        Range getClipRange()const override;
        void setClipRange(Range range) override;

        CameraType getCameraType()const override;
        void setCameraType(CameraType value) override;

        const Matrix& getViewMatrix()const override;
        const Matrix& getProjectionMatrix()const override;
        const Matrix& getViewProjectionMatrix()const override;

        Vec3 worldToScreen(const Vec3& position)const override;
        Vec3 screenToWorld(const Vec3& position,f32 distance)const override;





        //LayerMask getLayerMask()const override;
        //void setLayerMask(LayerMask mask) override;

        Rect getVieportRect()const override;
        void setVieportRect(Rect rect) override;

        void setRenderTarget(s32 displayNo) override;
        void setRenderTarget(const Ref<rhi::RenderTexture>&) override;
        auto getRenderTarget()const -> const Ref<rhi::RenderTexture>& override;


        f32 getAspect()const override;

        CameraType getType()const override;

        //auto getPriority()const override->CameraPriority;
        //void setPriority(CameraPriority priority) override;

        // Frustum
        // Range

        // 位置情報はTransform?


        void addRenderTargetEvent(rhi::TextureEventHandle& handle, rhi::TextureEventDelegate func)override;

    private:

        UPtr<RenderView> m_renderView;

        CameraState m_state;

        Transform   m_transform;
        Viewport    m_viewport;
        f32         m_fovY;
        CameraType  m_type;
        Rect        m_rect;
        //LayerMask   m_layerMask;
        //CameraPriority m_priority;

        Optional<s32> m_display;
        Ref<rhi::RenderTexture> m_renderTexture;

        Matrix      m_viewMatrix;
        Matrix      m_projMatrix;
        Matrix      m_viewprojMatrix;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}