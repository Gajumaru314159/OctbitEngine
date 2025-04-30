//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Render/RenderFeature.h>

namespace ob::graphics {

	//! @brief      描画シーン
	//! @ref 		RenderScene
	class RenderScene{
	public:

        RenderScene();
        ~RenderScene();

        //! @brief      RenderFeatureを追加する
        template<class T,class... Args>
        auto addFeature(Args&& ...args) -> std::enable_if_t<std::is_constructible<T, Args...>::value,T&>;

        //! @brief      RenderFeatureを見つける
        template<class T> T* findFeature()const;
        RenderFeature* findFeature(Type type)const;


        //! @brief      RenderViewを追加する
        void addView(RenderView* view);

        //! @brief      RenderViewを削除する
        void removeView(RenderView* view);


        //! @brief      解放時イベント
        void addReleasedEvent(RenderSceneEventHandle& handle, RenderSceneEventDelegate func);
        void addFeatureAddedEvent(RenderFeatureEventHandle& handle, RenderFeatureEventDelegate func);

        //! @brief      描画
        void render(FG& fg);

    private:
        void onFeatureAdded(RenderFeature& feature);
    private:
        HashMap<Type, UPtr<RenderFeature>>      m_features;
        Vector<RenderView*>                     m_views;

        RenderSceneEventNotifier                m_releasedNotifier;
        RenderFeatureEventNotifier              m_featureAddedNotifier;
	};


    //! @brief      RenderFeatureを追加する
    template<class T, class... Args>
    auto RenderScene::addFeature(Args&& ...args) -> std::enable_if_t<std::is_constructible<T,Args...>::value, T&> {
        auto& feature = m_features[Type::Get<T>()] = std::make_unique<T>(args...);
        onFeatureAdded(*feature);
        return *reinterpret_cast<T*>(feature.get());
    }

    //! @brief      RenderFeatureを見つける
    template<class T>
    T* RenderScene::findFeature()const {
        return reinterpret_cast<T*>(findFeature(Type::Get<T>()));
    }


}