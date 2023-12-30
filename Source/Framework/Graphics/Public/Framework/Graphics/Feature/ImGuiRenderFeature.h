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
	//! @brief      ImGui描画機能
	//@―---------------------------------------------------------------------------
	class ImGuiRenderFeature : public RenderFeature {
	public:

		ImGuiRenderFeature(RenderScene&);

		//@―---------------------------------------------------------------------------
		//! @brief  RenderStepを登録するための初期化処理
		//@―---------------------------------------------------------------------------
		void setupView(RenderStepSet&) override{}


	private:


	};

	//@―---------------------------------------------------------------------------
	//! @brief      ImGui描画ステップ
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
		void render(CommandRecorder&, const RenderArgs& args) override;

	private:

		Ref<rhi::FrameBuffer> m_frameBuffer;

	};


}