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
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class Camera {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

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
    public:

        static std::optional<Camera> GetMain();
        static Span<Camera> GetAll();

    private:

        Transform m_transform;

    };

}// namespcae ob