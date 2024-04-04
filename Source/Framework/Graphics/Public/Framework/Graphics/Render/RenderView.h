//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Platform/Window.h>

#include <Framework/Graphics/Render/RenderStep.h>

namespace ob::graphics {

    enum class RenderViewType {
        Camera,
        Shadow,
        ReflectionProbe,
    };

    struct RenderViewDesc {
        String          name;
        Size            size;
        RenderViewType  type;
    };
        
    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    class RenderView : public RefObject {
        friend class RenderScene;
    public:

        virtual ~RenderView() = default;

        //@―---------------------------------------------------------------------------
        //! @brief      名前を取得
        //@―---------------------------------------------------------------------------
        auto& getName()const { return m_desc.name; }

        void setPriority(s32 priority);
        s32 getPriority()const;

        // RenderTexture指定
        void setRenderTarget(const Ref<rhi::Display>& display);

        // Display設定
        auto getRenderTarget()const->const Ref<rhi::RenderTexture>;

        //@―---------------------------------------------------------------------------
        //! @brief      RenderFeatureを見つける
        //@―---------------------------------------------------------------------------
        template<class T> T* findStep()const;
        RenderStep* findStep(TypeId typId)const;

        auto& getScene() { return m_scene; }
        auto& getScene()const { return m_scene; }

        void applyDisplay(FG&);

    private:

        RenderView(const RenderViewDesc& desc,RenderScene& scene, RenderStepInjector& injector);

    private:
        RenderScene& m_scene;
        RenderViewDesc m_desc;
        Ref<rhi::Display> m_display;
        Ref<rhi::RenderTexture> m_texture;

        s32 m_priority;
        HashMap<TypeId,UPtr<RenderStep>> m_steps;

    };


    template<class T>
    T* RenderView::findStep()const {
        return reinterpret_cast<T*>(findStep(TypeId::Get<T>()));
    }

}