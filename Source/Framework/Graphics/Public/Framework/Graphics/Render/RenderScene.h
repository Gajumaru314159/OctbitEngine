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

#include <Framework/Graphics/Render/RenderPipeline.h>

namespace ob::graphics {

	enum class RenderViewId : u32 {};

	//@―---------------------------------------------------------------------------
	//! @brief      描画シーン
	//@―---------------------------------------------------------------------------
	class RenderScene : public RefObject {
		friend class Graphics;
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      RenderSceneを生成する
		//! @details	生成されたRenderSceneはデフォルトのRPIに自動登録されます。
		//!				登録先のRPIを変更する場合は通常のコンストラクタを使用してください。
		//@―---------------------------------------------------------------------------
		static Ref<RenderScene> Create(const RenderSceneDesc& desc,Graphics* graphics = nullptr);

	public:

		//@―---------------------------------------------------------------------------
		//! @brief      デストラクタ
		//@―---------------------------------------------------------------------------
		~RenderScene();

		//@―---------------------------------------------------------------------------
		//! @brief      シーンの削除をリクエスト
		//! @details	シーンの削除は即時は行われません。
		//!				Graphicsからは次フレームの開始時に登録解除されますが、RenderScene
		//!				への参照が残っている場合はオブジェクトの解放はさらに遅延されます。
		//@―---------------------------------------------------------------------------
		void requestDispose();

		bool isDisposeRequested()const;

		//@―---------------------------------------------------------------------------
		//! @brief      名前を取得
		//@―---------------------------------------------------------------------------
		auto& getName()const { return m_name; }

		//@―---------------------------------------------------------------------------
		//! @brief      描画タスクを記録する
		//@―---------------------------------------------------------------------------
		void render(FG&);

		//@―---------------------------------------------------------------------------
		//! @brief      RenderView を生成する
		//@―---------------------------------------------------------------------------
		auto createView(const RenderViewDesc& desc) -> Ref<RenderView>;
		
		//@―---------------------------------------------------------------------------
		//! @brief      RenderFeatureを見つける
		//@―---------------------------------------------------------------------------
		template<class T> T* findFeature()const;
		RenderFeature* findFeature(TypeId typId)const;

		//@―---------------------------------------------------------------------------
		//! @brief      RenderFeatureを走査する
		//@―---------------------------------------------------------------------------
		void visitFeatures(Func<void(RenderFeature&)>&&);

		//@―---------------------------------------------------------------------------
		//! @brief      全てのRenderFeatureを有効化する
		//@―---------------------------------------------------------------------------
		void activateAllFeature();


	private:

		//@―---------------------------------------------------------------------------
		//! @brief      RenderSceneを生成する
		//! @details	生成されたRenderSceneはRPIに登録する必要があります。
		//@―---------------------------------------------------------------------------
		RenderScene(const RenderSceneDesc& desc, Graphics& graphics);

	private:

		Graphics&								m_graphics;
		bool									m_disposeRequested = false;

		String									m_name;

		HashMap<TypeId, UPtr<RenderPipeline>>	m_pipelines;
		HashMap<TypeId, UPtr<RenderFeature>>	m_features;
		Array<Ref<RenderView>>					m_views;

	};


	template<class T>
	T* RenderScene::findFeature()const {
		return reinterpret_cast<T*>(findFeature(TypeId::Get<T>()));
	}

}