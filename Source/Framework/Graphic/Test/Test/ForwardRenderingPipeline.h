﻿//***********************************************************
//! @file
//! @brief		カメラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/IRenderPipeline.h>
#include <Framework/Graphic/CommandBuffer.h>
#include <Framework/Graphic/RenderContext.h>
#include <Framework/Graphic/Camera.h>
#include <Framework/RHI/RenderTarget.h>

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
		}

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		void render(RenderContext& context, Span<Camera> cameras) override {


			for (auto& camera : cameras) {

				context.setCamera(camera);

				s32 width, height;


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
						context.beginSubPass(indices);

						context.setCamera(camera);

						// Renderer描画
						//context.draw(/**/);

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

		CommandBuffer m_cmd;
		rhi::RenderTarget m_color;


	};

}// namespcae ob