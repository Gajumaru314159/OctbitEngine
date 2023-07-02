//***********************************************************
//! @file
//! @brief		UniversalRenderPipeline
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/RHI/FrameBuffer.h>
#include <Framework/RHI/RenderPass.h>

#include <Framework/Graphics/TextureManager.h>
#include <Framework/Graphics/Render/RenderContext.h>
#include <Framework/Graphics/Render/RenderStep.h>

namespace ob::graphics {

	struct UniversalRenderViewData {


		//Array<RenderFeature> features;
	};


	//@―---------------------------------------------------------------------------
	//! @brief      UniversalRenderPipeline
	//@―---------------------------------------------------------------------------
	class UniversalRenderView : public RenderView {
	public:

		UniversalRenderView();
		~UniversalRenderView();

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		void render(RenderContext& context, const CameraData& camera) override;

	private:

		TextureManager m_textureManager;
		Array<UPtr<RenderStep>> m_steps;

	};

}