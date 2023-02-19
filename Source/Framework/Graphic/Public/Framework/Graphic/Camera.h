//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/CameraType.h>
#include <Framework/RHI/Forward.h>

namespace ob::graphic {


    enum class Layer {

    };

    using LayerMask = BitFlags<Layer>;

    using CameraPriority = s32;


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

        //===============================================================
        // プロパティ
        //===============================================================
        f32 getFov()const;
        void setFov(f32 value);

        Range getClipRange()const;
        void setClipRange(Range range);

        CameraType getCameraType()const;
        void setCameraType(CameraType value);

        const Matrix& getViewMatrix()const;
        const Matrix& getProjectionMatrix()const;
        const Matrix& getViewProjectionMatrix()const;

        Vec3 worldToScreen(const Vec3& position)const;
        Vec3 screenToWorld(const Vec3& position)const;
        




        LayerMask getLayerMask()const;
        void setLayerMask(LayerMask mask);
        
        Rect getVieportRect();
        void setVieportRect(Rect rect);

        void setRenderTarget(s32 displayNo);
        void setRenderTarget(const Ref<rhi::RenderTexture>&);
        auto getRenderTarget()const -> const Ref<rhi::RenderTexture>&;


        f32 getAspect()const;

        CameraType getType()const;

        auto getPriority()const->CameraPriority;
        void setPriority(CameraPriority priority);

        // Frustum
        // Range

        // 位置情報はTransform?

    private:

        Camera();

    private:

        CameraState m_state;

        Transform   m_transform;
        Viewport    m_viewport;
        f32         m_fovY;
        CameraType  m_type;
        LayerMask   m_layerMask;
        Rect        m_rect;
        CameraPriority m_priority;

        Optional<s32> m_display;
        Ref<rhi::RenderTexture> m_renderTexture;

        Matrix      m_viewMatrix;
        Matrix      m_projMatrix;
        Matrix      m_viewprojMatrix;

    };

}// namespcae ob