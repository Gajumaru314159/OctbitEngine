//***********************************************************
//! @file
//! @brief		UniversalRenderPipeline
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Render/Sample/Universal/UniversalRenderView.h>
#include <Framework/Graphics/CameraType.h>

namespace ob::graphics {

	struct UniversalRenderPipelineData {
		enum class MSAA {
			Disabled,
			X2,
			X4,
			X8,
		};
		enum class Resolution {
			X1024,
			X2048
		};

		struct Quality {
			bool hdr;
			MSAA msaa;
			f32 renderScale;
		} quality;

		struct Lighting {
			bool castShadow;
			Resolution shadowResolution;
		} lighting;

		struct Shadow {
			f32 maxDistance;
		} shadow;
	};

	//@―---------------------------------------------------------------------------
	//! @brief      UniversalRenderPipeline
	//@―---------------------------------------------------------------------------
	class UniversalRenderPipeline : public RenderPipeline {
	public:

		UniversalRenderPipeline() {

			auto renderViewTest = std::make_unique< UniversalRenderView>();

		}

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

	private:

	};

}