//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Feature/ForwardRenderFeature.h>
#include <Framework/Graphics/Render/RenderView.h>
namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	ForwardRenderFeature::ForwardRenderFeature() {

	}

	//@―---------------------------------------------------------------------------
	//! @brief  描画処理
	//@―---------------------------------------------------------------------------
	void ForwardRenderFeature::setupView(RenderView& view) {
		view.addStep<ForwardOpaqueRenderStep>();
	}





	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	ForwardOpaqueRenderStep::ForwardOpaqueRenderStep() {

	}

	//@―---------------------------------------------------------------------------
	//! @brief  セットアップ
	//@―---------------------------------------------------------------------------
	void ForwardOpaqueRenderStep::setup(TextureManager& manager) {

		using namespace ob::rhi;

		auto gbuffer0 = manager.createTexture("GBuffer0", TextureFormat::RGB8);	// Diffuse	Flag
		auto gbuffer1 = manager.createTexture("GBuffer1", TextureFormat::RGB8);	// Metallic	Occlusion
		auto gbuffer2 = manager.createTexture("GBuffer2", TextureFormat::RGB8);	// Normal	Smoothness
		auto depth    = manager.createTexture("Depth"   , TextureFormat::D32);	// Depth

		RenderPassDescHelper rdesc;
		rdesc.name = TC("Forward");
		auto gbuffer0Idx = rdesc.addAttachment(TextureFormat::RGB8);
		auto gbuffer1Idx = rdesc.addAttachment(TextureFormat::RGB8);
		auto gbuffer2Idx = rdesc.addAttachment(TextureFormat::RGB8);
		auto depthIdx    = rdesc.addAttachment(TextureFormat::D32);
		rdesc.addSubpassXCD(TC("Forward"), { gbuffer0Idx,gbuffer1Idx,gbuffer2Idx }, depthIdx);

		FrameBufferDesc fdesc;
		fdesc.name = TC("Forword");
		fdesc.renderPass = Material::AddRenderPass(rdesc);
		fdesc.attachments = { gbuffer0,gbuffer1,gbuffer2,depth};

		m_frameBuffer = FrameBuffer::Create(fdesc);

	}

	//@―---------------------------------------------------------------------------
	//! @brief  描画処理
	//@―---------------------------------------------------------------------------
	void ForwardOpaqueRenderStep::render(RenderView& camera) {

		auto& context = camera.getRenderContext();

		context.beginRenderPass(m_frameBuffer);
		context.draw();
		//context.draw("Forward");
		context.endRenderPass();

	}


}