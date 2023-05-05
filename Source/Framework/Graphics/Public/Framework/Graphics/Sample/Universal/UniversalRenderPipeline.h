//***********************************************************
//! @file
//! @brief		UniversalRenderPipeline
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/RenderPipeline.h>
#include <Framework/Graphics/CameraType.h>
#include <Framework/Graphics/RenderView.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      UniversalRenderPipeline
	//@―---------------------------------------------------------------------------
	class UniversalRenderPipeline : public RenderPipeline {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		void render(RenderContext& context, Span<CameraData>  cameras) override {

			for (auto& camera : cameras) {
				if (camera.renderView) {
					camera.renderView->render(context, camera);
				}
			}

		}

	};

}