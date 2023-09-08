//***********************************************************
//! @file
//! @brief		UniversalRenderPipeline
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderStep.h>

#include <Framework/Graphics/Render/TextureManager.h>

namespace ob::graphics {


	//@―---------------------------------------------------------------------------
	//! @brief      UniversalRenderPipeline
	//@―---------------------------------------------------------------------------
	class ImGuiRenderFeature : public RenderFeature {
	public:

		ImGuiRenderFeature();

		//@―---------------------------------------------------------------------------
		//! @brief  RenderStepを登録するための初期化処理
		//@―---------------------------------------------------------------------------
		void setupView(RenderView& view) override;

	private:


	};

	//@―---------------------------------------------------------------------------
	//! @brief      UniversalRenderPipeline
	//@―---------------------------------------------------------------------------
	class ImGuiRenderStep : public RenderStep {
	public:

		ImGuiRenderStep();

		//@―---------------------------------------------------------------------------
		//! @brief  セットアップ
		//@―---------------------------------------------------------------------------
		void setup(TextureManager&) override;

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		void render(RenderView& view) override;

	private:

		Ref<rhi::FrameBuffer> m_frameBuffer;

	};


}