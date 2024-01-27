//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>

#include <Framework/Graphics/FrameGraph/FG.h>

#include <Framework/RHI/CommandList.h>

#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderScene.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画機能
	//! @details    O3DEでいうところのFeatureProcessor。
	//!				初期状態は非アクティブです。
	//@―---------------------------------------------------------------------------
	class SkyBoxRenderFeature : public RenderFeature {
	public:

		OB_RTTI();

		//@―---------------------------------------------------------------------------
		//! @brief      アクティブにする
		//@―---------------------------------------------------------------------------
		virtual void activate() {}

		//@―---------------------------------------------------------------------------
		//! @brief      非アクティブにする
		//@―---------------------------------------------------------------------------
		virtual void deactivate() {}

		//@―---------------------------------------------------------------------------
		//! @brief      シミュレートする
		//! @details	パーティクルの更新やアニメーションの更新などCPU上での更新処理を実行します。
		//@―---------------------------------------------------------------------------
		virtual void simulate() {}

		//@―---------------------------------------------------------------------------
		//! @brief      描画
		//@―---------------------------------------------------------------------------
		void render(FG& fg, FrameGraphResource target) {

			struct ImGuiData {
				FrameGraphResource target;
			};

			fg.addCallbackPass<ImGuiData>(
				"Sky",
				[=](FrameGraph::Builder& builder, ImGuiData& data) {
					data.target = builder.write(target);
				},
				[](const ImGuiData& data, FrameGraphPassResources& resources, void* ctx) {
					auto& cmd = *static_cast<rhi::CommandList*>(ctx);
					/*
					cmd.beginRenderPass();

					ImGui::BeginFrame();

					task

					ImGui::EndFrame(cmd);

					cmd.endRenderPass();

					*/
				}
			);
		}


	protected:
		SkyBoxRenderFeature(RenderScene& scene) :RenderFeature(scene) {}

	};

}