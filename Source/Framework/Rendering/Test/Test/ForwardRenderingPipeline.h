//***********************************************************
//! @file
//! @brief		カメラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Rendering/IRenderPipeline.h>
#include <Framework/Rendering/CommandBuffer.h>
#include <Framework/Rendering/RenderContext.h>
#include <Framework/Rendering/Camera.h>
#include <Framework/Graphic/RenderTarget.h>

namespace ob::rendering {

	class Attachment {
	public:
		Attachment(){}
		Attachment(graphic::TextureFormat format, Color color) {}
		Attachment(graphic::TextureFormat format,f32 depth){}
	};


	class ForwardRenderPipeline : public IRenderPipeline {
	public:

		ForwardRenderPipeline() {
			//m_cmd;
			m_attachments = {
				Attachment(graphic::TextureFormat::RGBA8,Color::Black),
				Attachment(graphic::TextureFormat::D32,0.0f)
			};
		}

		//@―---------------------------------------------------------------------------
		//! @brief  描画処理
		//@―---------------------------------------------------------------------------
		void render(RenderContext& context, Span<Camera> cameras) override {
			

			for (auto& camera : cameras) {

				context.setCamera(camera);

				s32 width, height;
				context.beginRenderPass(width,height, m_attachments);
				{
					context.beginSubPass();

					context.setCamera(camera);

					// Renderer描画
					//context.draw(/**/);

					context.endSubPass();
				}
				context.endRenderPass();

			}

			context.submit();

		}

	private:

		CommandBuffer m_cmd;
		graphic::RenderTarget m_color;

		Array<Attachment> m_attachments;

	};

}// namespcae ob