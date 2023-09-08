//***********************************************************
//! @file
//! @brief		UniversalRenderPipeline
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Render/RenderStep.h>
#include <Framework/Graphics/Render/TextureManager.h>
#include <Framework/Graphics/Material.h>
#include <Framework/Graphics/Render/RenderContext.h>
#include <Framework/RHI/Types/RenderPassDescHelper.h>
#include <Framework/RHI/Types/FrameBufferDesc.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/FrameBuffer.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      UniversalRenderPipeline
	//@―---------------------------------------------------------------------------
	class ForwardRenderFeature : public RenderFeature {
	public:

		ForwardRenderFeature();

		//@―---------------------------------------------------------------------------
		//! @brief  RenderStepを登録するための初期化処理
		//@―---------------------------------------------------------------------------
		void setupView(RenderView&) override;

	private:


	};

	//@―---------------------------------------------------------------------------
	//! @brief      ForwardOpaqueRenderStep
	//@―---------------------------------------------------------------------------
	class ForwardOpaqueRenderStep : public RenderStep {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		ForwardOpaqueRenderStep();

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