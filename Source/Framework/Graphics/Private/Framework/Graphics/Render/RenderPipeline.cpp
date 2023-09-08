//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderPipelineDesc.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Render/RenderStep.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	RenderPipeline::RenderPipeline(RenderPipelineDesc&& desc) {

		m_name = desc.name;
		m_features = std::move(desc.features);

	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	RenderPipeline::~RenderPipeline() {

	}

	//@―---------------------------------------------------------------------------
	//! @brief  Viewを追加
	//@―---------------------------------------------------------------------------
	void RenderPipeline::createView(String name) {

		// RenderStep生成
		RenderViewDesc desc;
		desc.name = name;
		desc.size = Size(1280, 720);

		auto view = std::make_unique<RenderView>(desc);

		for (auto& feature : m_features) {
			feature->setupView(*view);
		}

		view->startup();

		m_views.emplace_back(std::move(view));

	}

	//@―---------------------------------------------------------------------------
	//! @brief  描画処理
	//@―---------------------------------------------------------------------------
	void RenderPipeline::render() {

		// Viewをプライオリティでソート
		std::sort(m_views.begin(), m_views.end(),
			[](const UPtr<RenderView>& a, const UPtr<RenderView>& b) {
				return a->getPriority() < b->getPriority();
			}
		);

		// Featureの描画処理
		// 例：影・GIなどのカメラに依存しない描画
		for (auto& feature : m_features) {
			feature->render();
		}

		// TODO View毎に並列処理
		for (auto& view : m_views) {

			view->render();

			for (auto& step : m_steps) {
				step->render(*view);
			}

		}

	}

}