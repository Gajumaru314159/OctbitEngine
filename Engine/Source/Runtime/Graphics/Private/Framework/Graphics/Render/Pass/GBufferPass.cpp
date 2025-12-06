//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/Pass/GBufferPass.h>
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/Graphics/Render/Feature/MaterialRenderFeature.h>

using namespace ob::rhi;

namespace ob::graphics {

	EarlyZPass::EarlyZPass() {

	}

	EarlyZPass::Output EarlyZPass::render(FG& fg, RenderView& view, const Input& input)const {

		return fg.addPass<Output>(
			"EarlyZPass",
			[&](FGBuilder& builder, Output& output) {
				output.albedo = builder.write(input.albedo);
				output.normal = builder.read(input.normal);
				output.params = builder.read(input.params);
				output.depth = builder.write(input.depth);
			},
			[&](const Output& output, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
				if (auto feature = view.findFeature<MaterialRenderFeature>()) {

					MaterialBlockSet blocks(view);

					cmdList->pushMarker("EarlyZ");

					BeginPassParam param;
					param.colors.emplace_back(resources.getTexture(output.albedo), RenderPassBeforeAccessType::NoAccess, RenderPassAfterAccessType::NoAccess);
					param.depth = { resources.getTexture(output.depth), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve };

					cmdList->beginRenderPass(param);

					feature->render(cmdList, blocks, "EarlyZ");

					cmdList->endRenderPass();

					cmdList->popMarker();
				}
			}
		);

	}

	//--------------------------------

	OpaquePass::OpaquePass() {

	}

	OpaquePass::Output OpaquePass::render(FG& fg, RenderView& view, const Input& input)const {

		return fg.addPass<Output>(
			"Opaque",
			[&](FGBuilder& builder, Output& output) {
				output.albedo = builder.write(input.albedo);
				output.normal = builder.write(input.normal);
				output.params = builder.write(input.params);
				output.depth = builder.read(input.depth);
			},
			[&](const Output& output, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
				if (auto feature = view.findFeature<MaterialRenderFeature>()) {

					cmdList->pushMarker("Opaque");

					BeginPassParam param;
					param.colors.emplace_back(resources.getTexture(output.albedo), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);
					param.colors.emplace_back(resources.getTexture(output.normal), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);
					param.colors.emplace_back(resources.getTexture(output.params), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);
					param.depth = { resources.getTexture(output.depth), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve };

					cmdList->beginRenderPass(param);

					MaterialBlockSet blocks(view);
					feature->render(cmdList, blocks, "Opaque");

					cmdList->endRenderPass();

					cmdList->popMarker();
				}
			}
		);

	}

	//--------------------------------

	MaskedPass::MaskedPass() {

	}

	MaskedPass::Output MaskedPass::render(FG& fg, RenderView& view, const Input& input)const {

		return fg.addPass<Output>(
			"Masked",
			[&](FGBuilder& builder, Output& output) {
				output.albedo = builder.write(input.albedo);
				output.normal = builder.write(input.normal);
				output.params = builder.write(input.params);
				output.depth = builder.read(input.depth);
			},
			[&](const Output& output, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
				if (auto feature = view.findFeature<MaterialRenderFeature>()) {

					cmdList->pushMarker("Masked");

					BeginPassParam param;
					param.colors.emplace_back(resources.getTexture(output.albedo), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve);
					param.colors.emplace_back(resources.getTexture(output.normal), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve);
					param.colors.emplace_back(resources.getTexture(output.params), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve);
					param.depth = { resources.getTexture(output.depth), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve };

					cmdList->beginRenderPass(param);

					MaterialBlockSet blocks(view);
					feature->render(cmdList, blocks, "Masked");

					cmdList->endRenderPass();

					cmdList->popMarker();
				}
			}
		);

	}

}