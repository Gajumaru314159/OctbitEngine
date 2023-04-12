//***********************************************************
//! @file
//! @brief		カメラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/RenderPipeline.h>
#include <Framework/Graphics/CommandBuffer.h>
#include <Framework/Graphics/RenderContext.h>
#include <Framework/Graphics/Camera.h>
#include <Framework/Graphics/Material.h>
#include <Framework/Engine/Name.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/Types/RenderPassDescHelper.h>
#include <Framework/RHI/FrameBuffer.h>

namespace ob::graphics {

	class Attachment {
	public:
		Attachment() {}
		Attachment(rhi::ColorTextureFormat format, Color color) {}
		Attachment(rhi::DepthTextureFormat format, f32 depth) {}
	};


	class ForwardRenderPipeline : public RenderPipeline {
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
				Material::RegisterRenderPass(engine::Name(TC("Accumulate")), m_pbrRenderPass, 3);
			}

		}

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		void render(RenderContext& context, Span<Camera> cameras) override {
			using namespace ob::rhi;

			// グローバル変数設定



			for (auto& camera : cameras) {

				// カメラごとにフレームバッファが必要
				// カメラの描画対象がリサイズされた場合はFrameBufferもリサイズする必要がある
				// 描画はイミュータブルな操作であるためカメラごとに並列処理することが可能
				// ただしCommandBufferの実行自体はcamerasの順番に従って送信する必要がある



				auto size = camera.getRenderTarget()->size();
				s32 width = camera.getRenderTarget()->width();
				s32 height = camera.getRenderTarget()->height();

				camera.allocateRenderTexture();
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


	class DepthPrepassFeature : public RenderFeature{
	public:

		DepthPrepassFeature(RenderFeatureGroup& group) {

			using namespace ob::rhi;
			// RenderPass生成
			{
				RenderPassDescHelper desc;
				desc.name = TC("DepthPrepass");
				auto depth = desc.addAttachment(TextureFormat::D32);

				desc.addSubpassXXD(depth);

				m_renderPass = RenderPass::Create(desc);
				OB_ASSERT_EXPR(m_renderPass);

				Material::RegisterRenderPass(engine::Name(TC("DepthPrepass")), m_renderPass, 0);
			}

			auto size = group.getCamera()->getRenderTarget()->size();

			// サイズ連動テクスチャ生成
			{
				// 生成or他パスで生成済みであれば取得
				// 同名で同フォーマットがある場合のみ取得
				group.addRenderTexture();
			}


			{
				rhi::FrameBufferDesc desc;
				// Cameraのリサイズに合わせてテクスチャ生成
				desc.name = TC("DepthPrepassFeature");
				desc.renderPass = m_renderPass;

				// desc.attachments = {camera.getRenderTexture()};

				m_frameBuffer = FrameBuffer::Create(desc);
			}
		}

		//@―---------------------------------------------------------------------------
		//! @brief      描画処理
		//@―---------------------------------------------------------------------------
		virtual void render(RenderContext& context, const Ref<Camera>& camera) {

			if (!m_frameBuffer)
				return;

			context.beginRenderPass(m_frameBuffer);

			// RenderTagごとのRendererを集める
			// camera情報をもとにソートとフィルタ
			// 描画
			context.getRendererGroup();


		}

	private:

		Ref<rhi::RenderPass> m_renderPass;

		Ref<rhi::RenderTexture> m_depthTexture;
		Ref<rhi::FrameBuffer> m_frameBuffer;



	};

}