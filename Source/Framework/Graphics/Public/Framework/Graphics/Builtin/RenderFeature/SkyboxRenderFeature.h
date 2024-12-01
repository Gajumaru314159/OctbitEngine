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

	//! @brief      描画機能
	//! @details    O3DEでいうところのFeatureProcessor。
	//!				初期状態は非アクティブです。
	class SkyBoxRenderFeature : public RenderFeature {
	public:

		OB_RTTI();

		SkyBoxRenderFeature() {

		}

		//! @brief      描画
		void render(FG& fg, FrameGraphResource target) {

			struct Data {
				FrameGraphResource target;
			};

			fg.addPass<Data>(
				"Sky",
				[=](FrameGraph::Builder& builder, Data& data) {
					data.target = builder.write(target);
				},
				[](const Data& data, FrameGraphPassResources& resources, void* ctx) {
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

	private:


	};

}