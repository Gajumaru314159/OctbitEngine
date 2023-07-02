//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/Sample/Universal/UniversalRenderView.h>
#include <Framework/Graphics/Render/Sample/Universal/UniversalRenderOpaqueStep.h>

namespace ob::graphics {

	UniversalRenderView::UniversalRenderView() 
		: m_textureManager(Size(1280,720))
	{
		// コンストラクタでステップを受け取る
		m_steps.emplace_back(std::make_unique<OpaqueRenderStep>(m_textureManager));

	}

	UniversalRenderView::~UniversalRenderView() {
		LOG_INFO("削除");
	}

	//@―---------------------------------------------------------------------------
	//! @brief  描画処理
	//@―---------------------------------------------------------------------------
	void UniversalRenderView::render(RenderContext& context, const CameraData& camera) {
		for (auto& step : m_steps) {
			//step
		}
	}

}