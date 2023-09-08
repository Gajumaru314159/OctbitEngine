//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {

	Ref<RenderScene> RenderScene::Create(StringView name) {
		return nullptr;
	}

	RenderScene::RenderScene(StringView name) 
		: m_name(name)
	{

	}

	void RenderScene::simulate() {

		// TODO ジョブ分割
		for (auto& [id,pipeline]: m_pipelines) {
			//pipeline->simulate();
		}

	}

	void RenderScene::render() {

		// TODO ジョブ分割
		for (auto& [id, pipeline] : m_pipelines) {
			pipeline->render();
		}

	}

	RenderPipelineId RenderScene::addRenderPipeline(UPtr<RenderPipeline> pipeline) {
		static Atomic<s32> s_renderPipelineId = 0;
		auto id = static_cast<RenderPipelineId>(s_renderPipelineId.load());
		m_pipelines[id] = std::move(pipeline);
		
		m_pipelines[id]->createView(TC("TetView"));
		
		return id;
	}

}