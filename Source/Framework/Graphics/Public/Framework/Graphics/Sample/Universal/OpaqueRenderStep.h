//***********************************************************
//! @file
//! @brief		UniversalRenderPipeline
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/RenderStep.h>

#include <Framework/Graphics/TextureManager.h>
#include <Framework/Graphics/RenderContext.h>
#include <Framework/RHI/FrameBuffer.h>
#include <Framework/RHI/RenderPass.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      UniversalRenderPipeline
	//@―---------------------------------------------------------------------------
	class OpaqueRenderStep : public RenderStep {
	public:

		OpaqueRenderStep(TextureManager& manager) {

			using namespace ob::rhi;

			auto gbuffer0 = manager.createTexture("GBuffer0", TextureFormat::RGB8);	// Diffuse	Flag
			auto gbuffer1 = manager.createTexture("GBuffer1", TextureFormat::RGB8);	// Metallic	Occlusion
			auto gbuffer2 = manager.createTexture("GBuffer2", TextureFormat::RGB8);	// Normal	Smoothness
			auto depth = manager.createTexture("Depth", TextureFormat::D32);	// Depth

			RenderPassDescHelper rdesc;
			rdesc.name = TC("Forward");
			auto gbuffer0Idx = rdesc.addAttachment(TextureFormat::RGBA8);
			auto gbuffer1Idx = rdesc.addAttachment(TextureFormat::RGBA8);
			auto gbuffer2Idx = rdesc.addAttachment(TextureFormat::RGBA8);
			auto depthIdx = rdesc.addAttachment(TextureFormat::D32);
			rdesc.addSubpassXCD({ gbuffer0Idx,gbuffer1Idx,gbuffer2Idx }, depth);

			FrameBufferDesc fdesc;
			fdesc.name = TC("Forword");
			fdesc.renderPass = Material::AddRenderPass(rdesc);
			fdessc.attachment = {
				gbuffer0,
				gbuffer1,
				gbuffer2,
				depth,
			};

			m_frameBuffer = FrameBuffer::Create(fdesc);

		}

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		void execute(RenderContext& context, const CameraData& camera) override {

			context.beginRenderPass(m_frameBuffer);

			//context.draw("Forward");

		}

	private:

		Ref<rhi::FrameBuffer> m_frameBuffer;

	};

}