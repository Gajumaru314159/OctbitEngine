//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderPipeline.h>

namespace ob::graphics {

    class RenderFeatureSet {
    public:
        RenderFeature* add(Type type, UPtr<RenderFeature> feature) {
            m_features[type] = std::move(feature);
            return m_features[type].get();
        }
        template<class T,class... TArgs> auto add(TArgs&&... args) -> std::enable_if_t<std::is_constructible_v<T, TArgs...>, T*> {
            auto result = new T(std::forward<TArgs>(args)...);
            m_features[Type::Get<T>()].reset(result);
            return result;
        }
        template<class T> T* find()const {
			return reinterpret_cast<T*>(find(Type::Get<T>()));
        }
        RenderFeature* find(Type type)const {
            auto found = m_features.find(type);
            if (found == m_features.end())return nullptr;
            return found->second.get();
        }
    private:
        HashMap<Type, UPtr<RenderFeature>> m_features;
    };

	//! @brief      描画シーン
	//! @ref 		RenderScene
	class RenderScene{
	public:

        RenderScene();
        ~RenderScene();

        //! @brief      RenderFeatureを見つける
        template<class T> T* findFeature()const;
        RenderFeature* findFeature(Type type)const;

        void setPipeline(s32 index, Ref<RenderPipeline> pipeline);

        //! @brief      描画
        void render(FG& fg);

        //! @brief      解放時イベント
        void addReleasedEvent(RenderSceneEventHandle& handle, RenderSceneEventDelegate func);

    private:

		friend class RenderView;

        //! @brief      RenderViewを追加する
        void addView(RenderView* view);

        //! @brief      RenderViewを削除する
        void removeView(RenderView* view);

    private:

        RenderFeatureSet                        m_features;
        Map<s32,Ref<RenderPipeline>>            m_pipelines;
		Vector<RenderView*>                     m_views;

        RenderSceneEventNotifier                m_releasedNotifier;
	};


    //! @brief      RenderFeatureを見つける
    template<class T>
    T* RenderScene::findFeature()const {
        return reinterpret_cast<T*>(findFeature(Type::Get<T>()));
    }

}