//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderExecutor.h>
#include <Framework/Graphics/Command/CommandRecorder.h>
#include <Framework/Graphics/RPI.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画シーンを生成
	//@―---------------------------------------------------------------------------
	Ref<RenderScene> RenderScene::Create(const RenderSceneDesc& desc) {
		if (auto system = RPI::Get()) {
			Ref<RenderScene> scene = new RenderScene(desc);
			system->addScene(scene);
			return scene;
		}
		return nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画シーンを生成
	//@―---------------------------------------------------------------------------
	RenderScene::RenderScene(const RenderSceneDesc& desc)
		: m_name(desc.name)
	{
		// RenderFeature生成
		auto features = desc.features.create(*this);
		for (auto& feature : features) {
			m_features[feature->getTypeId()] = std::move(feature);
		}

		// RenderPipeline生成
		auto pipelines = desc.pipelines.create(*this);
		for (auto& pipeline : pipelines) {
			m_pipelines.push_back(std::move(pipeline));
		}

		// RenderExecutor生成
		{
			RenderExecutorDesc desc;
			m_executor = std::make_unique<RenderExecutor>(desc);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void RenderScene::render(CommandStorage& commandStorage) {

		JobContext* ctx = nullptr;
		
		CommandRecorder recorder{ commandStorage };

		// RenderPipeline毎のコマンドを記録
		for (auto& pipeline : m_pipelines) {
			pipeline->render(recorder);
		}

		// RenderFeature毎のコマンドを記録
		for (auto& [typeId, feature] : m_features) {
			feature->render(recorder);
		}

		// RenderView毎の描画コマンドを記録
		for (auto& view : m_views) {
			view->render(recorder);
		}

		// ctx->dispatch();
	}

	//@―---------------------------------------------------------------------------
	//! @brief      RenderFeatureを見つける
	//@―---------------------------------------------------------------------------
	RenderFeature* RenderScene::findFeature(TypeId typId)const {
		auto found = m_features.find(typId);
		if (found == m_features.end())return nullptr;
		return found->second.get();
	}

	//@―---------------------------------------------------------------------------
	//! @brief      RenderFeatureを走査する
	//@―---------------------------------------------------------------------------
	void RenderScene::visitFeatures(Func<void(RenderFeature&)>&& visitor) {
		if (!visitor)return;
		for (auto& [typeId,feature] : m_features) {
			visitor(*feature);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      全てのRenderFeatureを有効にする
	//@―---------------------------------------------------------------------------
	void RenderScene::activateAllFeature() {
		for (auto& [typeId, feature] : m_features) {
			feature->activate();
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      RenderFeatureを見つける
	//@―---------------------------------------------------------------------------
	void RenderScene::addView(Ref<RenderView> view) {
		m_views.push_back(view);
	}

}