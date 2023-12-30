//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/Graphics/Render/RenderSceneDesc.h>
#include <Framework/Graphics/Render/RenderFeature.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画シーン
	//@―---------------------------------------------------------------------------
	class RenderScene {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      RenderSceneを生成する
		//! @details	生成されたRenderSceneはデフォルトのRPIに自動登録されます。
		//!				登録先のRPIを変更する場合は通常のコンストラクタを使用してください。
		//@―---------------------------------------------------------------------------
		static Ref<RenderScene> Create(const RenderSceneDesc& desc);

	public:

		//@―---------------------------------------------------------------------------
		//! @brief      RenderSceneを生成する
		//! @details	生成されたRenderSceneはRPIに登録する必要があります。
		//@―---------------------------------------------------------------------------
		RenderScene(const RenderSceneDesc& desc);

		//@―---------------------------------------------------------------------------
		//! @brief      名前を取得
		//@―---------------------------------------------------------------------------
		auto& getName()const { return m_name; }

		//@―---------------------------------------------------------------------------
		//! @brief      描画タスクを記録する
		//@―---------------------------------------------------------------------------
		void render(CommandStorage&);


		//@―---------------------------------------------------------------------------
		//! @brief      RenderFeatureを見つける
		//@―---------------------------------------------------------------------------
		template<class T> T* findFeature()const;
		//@―---------------------------------------------------------------------------
		//! @brief      RenderFeatureを見つける
		//@―---------------------------------------------------------------------------
		RenderFeature* findFeature(TypeId typId)const;

		//@―---------------------------------------------------------------------------
		//! @brief      RenderFeatureを走査する
		//@―---------------------------------------------------------------------------
		void visitFeatures(Func<void(RenderFeature&)>&&);

		//@―---------------------------------------------------------------------------
		//! @brief      全てのRenderFeatureを有効化する
		//@―---------------------------------------------------------------------------
		void activateAllFeature();


		//@―---------------------------------------------------------------------------
		//! @brief      ビューを生成する 
		//@―---------------------------------------------------------------------------
		template<class T>
		Ref<RenderView> createView();
		void addView(Ref<RenderView>);

	private:
		String m_name;

		HashMap<TypeId,UPtr<RenderFeature>> m_features;
		Array<UPtr<RenderPipeline>>			m_pipelines;
		Array<Ref<RenderView>>				m_views;

		// CullingScene
		UPtr<RenderExecutor>				m_executor;

		// RenderItem[]
		// RenderView[]
		// RenderPipeline[]

	};


	template<class T>
	Ref<RenderView> RenderScene::createView() {
		static_assert(std::is_base_of<RenderPipeline, T>::value,"T is not RenderPipeline");
		static_assert(false);
		return nullptr;
		//return createView(TypeId::Get())
	}


	template<class T>
	T* RenderScene::findFeature()const {
		return reinterpret_cast<T*>(findFeature(TypeId::Get<T>());
	}

}