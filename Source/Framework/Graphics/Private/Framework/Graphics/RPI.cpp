//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/RPI.h>
#include <Framework/Graphics/Render/RenderTask.h>
#include <Framework/RHI/RHI.h>

namespace ob::graphics {

	class RPIImpl :public RPI {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		RPIImpl(rhi::RHI& rhi)
			: m_rhi(rhi)
		{

		}

		//@―---------------------------------------------------------------------------
		//! @brief      描画タスクを追加
		//@―---------------------------------------------------------------------------
		void addRenderTask(RenderTask::RenderTaskDelegate func) {
			auto task = std::make_unique<RenderTask>(func);
		}


		//@―---------------------------------------------------------------------------
		//! @brief      ゲームループごとの更新を実行する
		//@―---------------------------------------------------------------------------
		void simulate() override {
			
		}

		//@―---------------------------------------------------------------------------
		//! @brief      描画ループの更新をする
		//@―---------------------------------------------------------------------------
		void render() override {

			// m_rhi.update();
		}

		//@―---------------------------------------------------------------------------
		//! @brief      RenderFeatureを取得する
		//@―---------------------------------------------------------------------------
		RenderFeature* findFeature(const TypeId& type)const override {
			return try_find(m_featureMap, type, nullptr);
		}

	private:
		rhi::RHI& m_rhi;
		Array<Ref<RenderScene>> m_scenes;

		using RenderFeatureList = Array<UPtr<RenderFeature>>;
		using RenderFeatureMap= HashMap<TypeId,RenderFeature*>;
		using RenderViewList = Array<UPtr<RenderView>>;


		RenderFeatureList m_features;
		RenderFeatureMap m_featureMap;
		RenderViewList m_views;
	};

}