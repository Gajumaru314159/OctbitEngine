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
#include <Framework/Graphics/Command/CommandRecorder.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	ImGuiRenderFeature::ImGuiRenderFeature(RenderScene& scene)
		: RenderFeature(scene)
	{

	}

	//@―---------------------------------------------------------------------------
	//! @brief  ビュー設定
	//@―---------------------------------------------------------------------------
	void ImGuiRenderFeature::setupView(RenderStepSet& set) {
		set.add<ImGuiRenderStep>();
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
	void ImGuiRenderStep::render(CommandRecorder& recorder, const RenderArgs& args) {

		recorder.beginRenderPass(m_frameBuffer);
		
		// TODO ImGuiをRHIではなくRPI依存にする

		//context.executeCustomCommand();
		//context.draw("Forward");

		recorder.endRenderPass();

	}
}