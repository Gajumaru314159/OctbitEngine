//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Feature/ImGuiRenderFeature.h>
#include <Framework/Graphics/Material.h>
#include <Framework/RHI/Types/RenderPassDescHelper.h>
#include <Framework/RHI/Types/FrameBufferDesc.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/FrameBuffer.h>

#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	ImGuiRenderFeature::ImGuiRenderFeature() {

	}

	//@―---------------------------------------------------------------------------
	//! @brief  ビュー設定
	//@―---------------------------------------------------------------------------
	void ImGuiRenderFeature::setupView(RenderView& view) {
		view.addStep<ImGuiRenderStep>();
	}



	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	ImGuiRenderStep::ImGuiRenderStep() {

	}

	//@―---------------------------------------------------------------------------
	//! @brief  セットアップ
	//@―---------------------------------------------------------------------------
	void ImGuiRenderStep::setup(TextureManager& manager) {

		using namespace ob::rhi;

		auto accumulate = manager.findTexture("Accumulate");

		if (!accumulate)return;

		RenderPassDescHelper rdesc;
		rdesc.name = TC("ImGui");
		auto accumulateIdx = rdesc.addAttachment(accumulate->format());
		rdesc.addSubpassXCX(TC("ImGui"), { accumulateIdx });

		FrameBufferDesc fdesc;
		fdesc.name = TC("ImGui");
		fdesc.renderPass = Material::AddRenderPass(rdesc);
		fdesc.attachments = { accumulate };

		m_frameBuffer = FrameBuffer::Create(fdesc);

	}

	//@―---------------------------------------------------------------------------
	//! @brief  描画処理
	//@―---------------------------------------------------------------------------
	void Render_ImGui() {

		context.beginRenderPass(m_frameBuffer);
		
		//context.executeCustomCommand();
		//context.draw("Forward");

		context.endRenderPass();

	}
}