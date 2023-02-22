//***********************************************************
//! @file
//! @brief		カメラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/IRenderPipeline.h>
#include <Framework/Graphic/CommandBuffer.h>
#include <Framework/Graphic/RenderContext.h>
#include <Framework/Graphic/Camera.h>
#include <Framework/Graphic/Material.h>
#include <Framework/Engine/Name.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/Types/RenderPassDescHelper.h>

namespace ob::graphic {

	class Attachment {
	public:
		Attachment() {}
		Attachment(rhi::ColorTextureFormat format, Color color) {}
		Attachment(rhi::DepthTextureFormat format, f32 depth) {}
	};


	class ForwardRenderPipeline : public IRenderPipeline {
	public:

		ForwardRenderPipeline() {
			using namespace ob::rhi;

			// PBR
			{
				RenderPassDescHelper desc;
				desc.name = TC("Forward");
				auto albedo = desc.addAttachment(TextureFormat::RGBA8);
				auto gbuffer0 = desc.addAttachment(TextureFormat::RGBA8);
				auto depth = desc.addAttachment(TextureFormat::D32);
				auto accumulate = desc.addAttachment(TextureFormat::RGBA8);

				desc.addSubpassXXD(depth);
				desc.addSubpassXCD({ albedo,gbuffer0 }, depth);
				desc.addSubpassXCD({ albedo,gbuffer0 }, depth);
				desc.addSubpassICX({ albedo,gbuffer0 }, { accumulate });

				m_pbrRenderPass = RenderPass::Create(desc);
				OB_ASSERT_EXPR(m_pbrRenderPass);

				Material::RegisterRenderPass(engine::Name(TC("EarlyDepth")), m_pbrRenderPass, 0);
				Material::RegisterRenderPass(engine::Name(TC("Opaque")), m_pbrRenderPass, 1);
				Material::RegisterRenderPass(engine::Name(TC("Transpaternt")), m_pbrRenderPass, 2);
				Material::RegisterRenderPass(engine::Name(TC("Accumulate")), m_pbrRenderPass, 2);
			}

		}

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		void render(RenderContext& context, Span<Camera> cameras) override {
			using namespace ob::rhi;

			for (auto& camera : cameras) {

				auto size = camera.getRenderTarget()->size();
				s32 width = camera.getRenderTarget()->width();
				s32 height = camera.getRenderTarget()->height();
				/*

				Ref<RenderTexture> albedoRT = context.allocateRenderTexture(size, TextureFormat::RGBA8);
				Ref<RenderTexture> gbuffer0RT = context.allocateRenderTexture(size, TextureFormat::RGBA8);
				Ref<RenderTexture> depthRT = context.allocateRenderTexture(size, TextureFormat::D32);
				Ref<RenderTexture> accumulateRT = context.allocateRenderTexture(size, TextureFormat::RGBA8);
				auto cameraRT = camera.getRenderTexture();

				FrameBufferDesc desc;
				desc.renderPass = m_pbrRenderPass;
				desc.attachments = {
					albedoRT,
					gbuffer0RT,
					depthRT,
					cameraRT,
				};
				Ref<FrameBuffer> frameBuffer = FrameBuffer::Create();

				context.beginRenderPass(frameBuffer);

				//

				context.nextSubPass();

				//

				context.endRenderPass();

				*/
				context.setCamera(camera);



				// デプスあり
				{
					Attachment colors[] = {
						Attachment(rhi::ColorTextureFormat::RGBA16,Color::Black),
						Attachment(rhi::ColorTextureFormat::RG16,Color::Normal)
					};
					Attachment depth(rhi::DepthTextureFormat::D32, 0.f);


					context.beginRenderPass(width, height, colors);
					{
						s32 indices[]{ 0};
						context.beginSubPass(indices, {});

						context.setCamera(camera);

						// Renderer描画
						//context.draw(/**/);

						engine::Name names[]{
							engine::Name(TC("Opaque")),
							engine::Name(TC("HairOpaque")),
						};

						engine::Name renderTag(TC("EarlyDepth"));


						// 指定したレンダータグのRendererを収集
						// RenderPassは設定済み
						// 実行順はSubpassに合わせる必要がある
						context.getRendererGroup(renderTag)
							.draw();

						context.endSubPass();
					}
					context.endRenderPass();
				}
				// デプスなし
				{
					Attachment colors[] = {
						Attachment(rhi::ColorTextureFormat::RGB8,Color::White),
					};
					context.beginRenderPass(width, height, colors);
					// 省略
					context.endRenderPass();
				}

			}

			context.submit();

		}

	private:

		Ref<rhi::RenderPass> m_pbrRenderPass;

	};

}// namespcae ob