//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/CameraType.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Texture.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      カメラ
    //! @details    CameraCompoentとは異なりReflectionCaptureやShadowCaptureでも
    //!             使用されます。
    //@―---------------------------------------------------------------------------
    class Camera: public RefObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      生成
        //@―---------------------------------------------------------------------------
        template<class T,class... TArgs>
        static Ref<Camera> Create(TArgs&&...);

        static Ref<Camera> Create(UPtr<RenderView>);

    public:

        virtual ~Camera();

        //@―---------------------------------------------------------------------------
        //! @brief      Field of View 取得
        //@―---------------------------------------------------------------------------
        virtual f32 getFov()const = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      Field of View 設定
        //@―---------------------------------------------------------------------------
        virtual void setFov(f32 value) = 0;

        virtual Range getClipRange()const = 0;
        virtual void setClipRange(Range range) = 0;

        virtual CameraType getCameraType()const = 0;
        virtual void setCameraType(CameraType value) = 0;


        //@―---------------------------------------------------------------------------
        //! @brief      ビュー行列取得
        //@―---------------------------------------------------------------------------
        virtual const Matrix& getViewMatrix()const = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      プロジェクション行列取得
        //@―---------------------------------------------------------------------------
        virtual const Matrix& getProjectionMatrix()const = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      ビュープロジェクション行列
        //@―---------------------------------------------------------------------------
        virtual const Matrix& getViewProjectionMatrix()const = 0;


        //@―---------------------------------------------------------------------------
        //! @brief      ワールド座標をスクリーン座標に変換
        //! @details    戻り値のZ座標が負の場合ワールド座標はカメラの背面にあることを示します。
        //@―---------------------------------------------------------------------------
        virtual Vec3 worldToScreen(const Vec3& position)const = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      スクリーン座標をワールド座標に変換
        //@―---------------------------------------------------------------------------
        virtual Vec3 screenToWorld(const Vec3& position,f32 distance)const = 0;
        

        // WorldMatrix
        // CameraMatrix
        // Rect


        //virtual LayerMask getLayerMask()const;
        //virtual void setLayerMask(LayerMask mask);

        //@―---------------------------------------------------------------------------
        //! @brief      カメラの描画範囲矩形を[0,1]で指定します。
        //@―---------------------------------------------------------------------------
        virtual Rect getVieportRect()const = 0;
        virtual void setVieportRect(Rect rect) = 0;

        //@―---------------------------------------------------------------------------
        //! @brief      カメラの描画先
        //@―---------------------------------------------------------------------------
        virtual void setRenderTarget(s32 displayNo) = 0;
        virtual void setRenderTarget(const Ref<rhi::RenderTexture>&) = 0;
        virtual auto getRenderTarget()const -> const Ref<rhi::RenderTexture>& = 0;


        virtual f32 getAspect()const = 0;

        virtual CameraType getType()const = 0;

        //virtual auto getPriority()const->CameraPriority;
        //virtual void setPriority(CameraPriority priority);

        // Frustum
        // Range

        // 位置情報はTransform?

        virtual void addRenderTargetEvent(rhi::TextureEventHandle& handle,rhi::TextureEventDelegate func) = 0;


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


    template<class T,class... TArgs>
    Ref<Camera> Camera::Create(TArgs&&... args) {
        return Camera::Create(new T(args...));
    }

}