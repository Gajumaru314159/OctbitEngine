//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Platform/Window.h>

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
        Ref<rhi::Display> display;
    };

    enum class RenderViewId{};

    
    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    class RenderView {
        friend class RenderScene;
    public:

        virtual ~RenderView();

        auto getId()const->RenderViewId;

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

    private:

        RenderView(const RenderViewDesc& desc,RenderScene& scene);

    private:
        RenderScene& m_scene;
        RenderViewDesc m_desc;
        RenderViewId m_id;
        s32 m_priority;

        HashMap<u32, UPtr<RenderStep>> m_steps;

    };


    template<class T>
    T* RenderView::findStep()const {
        return reinterpret_cast<T*>(findStep(TypeId::Get<T>()));
    }

}