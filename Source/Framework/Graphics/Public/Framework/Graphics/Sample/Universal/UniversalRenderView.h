//***********************************************************
//! @file
//! @brief		UniversalRenderPipeline
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/RenderView.h>
#include <Framework/RHI/FrameBuffer.h>
#include <Framework/RHI/RenderPass.h>

#include <Framework/Graphics/TextureManager.h>
#include <Framework/Graphics/RenderContext.h>
#include <Framework/Graphics/RenderStep.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      UniversalRenderPipeline
	//@―---------------------------------------------------------------------------
	class UniversalRenderView : public RenderView {
	public:

		UniversalRenderView() {


		}

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		void render(RenderContext& context, const CameraData& camera) override{
			for (auto& step : m_steps) {
				step
			}
		}

	private:

		Array<UPtr<RenderStep>> m_steps;

	};

}