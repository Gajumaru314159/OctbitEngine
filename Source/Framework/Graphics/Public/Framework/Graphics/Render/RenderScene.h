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

	//@―---------------------------------------------------------------------------
	//! @brief      描画シーン
	//@―---------------------------------------------------------------------------
	class RenderScene{
	public:

        RenderScene();
        ~RenderScene();

        //@―---------------------------------------------------------------------------
        //! @brief      RenderFeatureを追加する
        //@―---------------------------------------------------------------------------
        template<class T,class... Args>
        void addFeature(Args&& ...args);

        //@―---------------------------------------------------------------------------
        //! @brief      RenderFeatureを見つける
        //@―---------------------------------------------------------------------------
        template<class T> T* findFeature()const;
        RenderFeature* findFeature(TypeId typeId)const;


        //@―---------------------------------------------------------------------------
        //! @brief      RenderViewを追加する
        //@―---------------------------------------------------------------------------
        void addView(RenderView* view);

        //@―---------------------------------------------------------------------------
        //! @brief      RenderViewを削除する
        //@―---------------------------------------------------------------------------
        void removeView(RenderView* view);


        //@―---------------------------------------------------------------------------
        //! @brief      解放時イベント
        //@―---------------------------------------------------------------------------
        void addReleasedEvent(RenderSceneEventHandle& handle, RenderSceneEventDelegate func);
        void addFeatureAddedEvent(RenderFeatureEventHandle& handle, RenderFeatureEventDelegate func);

        //@―---------------------------------------------------------------------------
        //! @brief      描画
        //@―---------------------------------------------------------------------------
        void render(FG& fg);

    private:
        void onFeatureAdded(RenderFeature& feature);
    private:
        HashMap<TypeId, UPtr<RenderFeature>> m_features;
        Array<RenderView*> m_views;
        RenderSceneEventNotifier    m_releasedNotifier;
        RenderFeatureEventNotifier  m_featureAddedNotifier;
	};


    //@―---------------------------------------------------------------------------
    //! @brief      RenderFeatureを追加する
    //@―---------------------------------------------------------------------------
    template<class T, class... Args>
    void RenderScene::addFeature(Args&& ...args) {
        auto& feature = m_features[TypeId::Get<T>()] = std::make_unique<T>(args...);
        onFeatureAdded(*feature);
    }

    //@―---------------------------------------------------------------------------
    //! @brief      RenderFeatureを見つける
    //@―---------------------------------------------------------------------------
    template<class T>
    T* RenderScene::findFeature()const {
        return reinterpret_cast<T*>(findFeature(TypeId::Get<T>()));
    }


}