//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Render/RenderScene.h>

namespace ob::graphics {


    class RenderView {
    public:
        RenderView(RenderScene& scene, StringView name);
        ~RenderView();

        void setDisplay(const Ref<Display>& display);
        auto getDisplay()const -> const Ref<rhi::Display>;
        void setRenderTexture(const Ref<RenderTexture>& renderTexture);
        auto getRenderTexture()const->const Ref<rhi::RenderTexture>;

        template<class T, class... Args>
        void setPipeline(Args&&... args) {
            m_pipeline = std::make_unique<T>(args...);
        }

        template<class T>
        T* getPipeline() {
            if (!m_pipeline)return nullptr;
            if (m_pipeline->getTypeId() != TypeId::Get<T>())return;
            return reinterpret_cast<T*>(m_pipeline.get());
        }

        void render(FG& fg);

        //@―---------------------------------------------------------------------------
        //! @brief      解放時イベント
        //@―---------------------------------------------------------------------------
        void addReleasedEvent(RenderViewEventHandle& handle, RenderViewEventDelegate func);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief      RenderFeatureを見つける
        //@―---------------------------------------------------------------------------
        template<class T> T* findFeature()const;
        RenderFeature* findFeature(TypeId typeId)const;

    private:
        void clearTarget();
        void onSceneReleased(RenderScene& scene);
    private:
        String                  m_name;
        RenderScene&            m_scene;
        Ref<Display>            m_display;
        Ref<RenderTexture>      m_renderTexture;
        UPtr<RenderPipeline>    m_pipeline;
        RenderSceneEventHandle  m_hRelease;
        RenderViewEventNotifier m_releasedNotifier;
    };

    template<class T>
    T* RenderView::findFeature()const {
        return reinterpret_cast<T*>(findFeature(TypeId::Get<T>()));
    }

}