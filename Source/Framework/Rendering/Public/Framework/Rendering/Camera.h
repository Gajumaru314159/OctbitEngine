//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Rendering/Camera/CameraType.h>

namespace ob::rendering {


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

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        void render();


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

        void setRenderTarget(const RenderTexture&);
        auto getRenderTarget()const -> const RenderTexture&;
        void setRenderTarget();


        f32 getAspect()const;

        CameraType getCameraType()const;

        auto getPriority()const->CameraPriority;
        void setPriority(CameraPriority priority);

        // Frustum
        // Range

        // 位置情報はTransform?
    public:

        static std::optional<Camera> GetMain();
        static Span<Camera> GetAll();

    private:



    };

}// namespcae ob