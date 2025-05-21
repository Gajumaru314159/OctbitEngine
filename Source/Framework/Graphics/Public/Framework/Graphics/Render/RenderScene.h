//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Render/RenderFeature.h>

#include <Framework/Graphics/Render/RenderSceneDesc.h>
#include <Framework/Graphics/Render/RenderPipeline.h>

namespace ob::graphics {

	//! @brief      描画シーン
	//! @ref 		RenderScene
	class RenderScene{
	public:

        RenderScene(const RenderSceneDesc& desc);
        ~RenderScene();

        //! @brief      RenderFeatureを見つける
        template<class T> T* findFeature()const;
        RenderFeature* findFeature(Type type)const;

        //! @brief      描画
        void render(FG& fg);

        //! @brief      解放時イベント
        void addReleasedEvent(RenderSceneEventHandle& handle, RenderSceneEventDelegate func);

    private:
		friend class RenderView;

        //! @brief      RenderViewを追加する
        void addView(RenderView* view,StringView pipeline);

        //! @brief      RenderViewを削除する
        void removeView(RenderView* view);

    private:

        Vector<UPtr<RenderFeature>>             m_features;
        HashMap<Type, RenderFeature*>           m_featuresByType;

        Map<String,RenderPipeline,std::less<>>  m_pipelines;

        RenderSceneEventNotifier                m_releasedNotifier;
	};

    //! @brief      RenderFeatureを見つける
    template<class T>
    T* RenderScene::findFeature()const {
        return reinterpret_cast<T*>(findFeature(Type::Get<T>()));
    }

}