//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/Pass/OutputPass.h>
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/RHI/SwapChain.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/Graphics/Render/Feature/MaterialRenderFeature.h>

using namespace ob::rhi;

namespace ob::graphics {

	Optional<Size> OutputViewData::size() const {
		if (swapchain) return swapchain->getDesc().size;
		if (texture) return texture->size();
		return std::nullopt;
	}

	OutputPass::Output OutputPass::render(FG& fg, RenderView& view, Input input) const {

		auto& output = view.get<OutputViewData>();
		auto& material = view.get<MaterialRFData>();

		material.block.setMatrix("MatrixV", Matrix::Identity);

		return fg.addPass<Output>(
			"CameraPass",
			[&](FGBuilder& builder, Output& output) {
				output.color = builder.read(input.color);
				builder.setSideEffect();
			},
			[&](const Output& output, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
				auto albedo = resources.getTexture(output.color);

				auto& data = view.get<OutputViewData>();

				if (data.swapchain) {
					cmdList->applySwapChain(data.swapchain, albedo);
				}
				else if (data.texture) {
					// TODO コピー処理
					OB_NOTIMPLEMENTED();
				}

			}
		);

	}
}