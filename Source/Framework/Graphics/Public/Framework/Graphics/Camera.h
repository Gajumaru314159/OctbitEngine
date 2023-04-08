//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/CameraType.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Texture.h>

namespace ob::graphics {

    class RenderTexture;
    class Display;

    //@―---------------------------------------------------------------------------
    //! @brief      カメラ
    //! @details    CameraCompoentとは異なりReflectionCaptureやShadowCaptureでも
    //!             使用されます。
    //@―---------------------------------------------------------------------------
    class Camera:public RefObject {
    public:

        static Ref<Camera> Create();

        static Ref<Camera> GetMain();
        static Array<Ref<Camera>> GetAll();

    public:

        virtual ~Camera() = default;

        virtual f32 getFov()const = 0;
        virtual void setFov(f32 value) = 0;

        virtual Range getClipRange()const = 0;
        virtual void setClipRange(Range range) = 0;

        virtual CameraType getCameraType()const = 0;
        virtual void setCameraType(CameraType value) = 0;

        virtual const Matrix& getViewMatrix()const = 0;
        virtual const Matrix& getProjectionMatrix()const = 0;
        virtual const Matrix& getViewProjectionMatrix()const = 0;

        virtual Vec3 worldToScreen(const Vec3& position)const = 0;
        virtual Vec3 screenToWorld(const Vec3& position)const = 0;
        

        // WorldMatrix
        // CameraMatrix
        // Rect


        //virtual LayerMask getLayerMask()const;
        //virtual void setLayerMask(LayerMask mask);
        
        virtual Rect getVieportRect();
        virtual void setVieportRect(Rect rect);

        virtual void setRenderTarget(s32 displayNo);
        virtual void setRenderTarget(const Ref<rhi::RenderTexture>&);
        virtual auto getRenderTarget()const -> const Ref<rhi::RenderTexture>&;


        virtual f32 getAspect()const;

        virtual CameraType getType()const;

        //virtual auto getPriority()const->CameraPriority;
        //virtual void setPriority(CameraPriority priority);

        // Frustum
        // Range

        // 位置情報はTransform?

        virtual Ref<rhi::RenderTexture> allocateRenderTexture();

        virtual void addRenderTargetEvent(rhi::TextureEventHandle& handle,rhi::TextureEventDelegate func);

    protected:

        Camera() = default;

    private:

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

}// namespcae ob