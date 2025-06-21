//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/SwapChain.h>
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderPass.h>
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>
#include <Framework/Graphics/Material/MaterialPropertiesDesc.h>

namespace ob::graphics {

	class CameraPass;

	struct OutputViewData {
		Ref<rhi::SwapChain>		swapchain;			//!< 描画先
		Ref<rhi::RenderTexture> texture;			//!< 描画先
		Rect                    rect;				//!< 描画範囲の矩形
		Viewport				viewport;			//!< 描画範囲

		Optional<Size> size()const {
			if (swapchain) return swapchain->getDesc().size;
			if (texture) return texture->size();
			return std::nullopt;
		}
	};

	//! @brief      マテリアル描画機能
	class CameraRenderFeature : public RenderFeature {
	public:
		static MaterialPropertiesSetDesc GetProperties() {
			MaterialPropertiesSetDesc desc;
			desc.global.matrices = {
				"MatrixTest"
			};
			desc.view.matrices = {
				"MatrixV",
				"MatrixP",
				"MatrixVP",
				"MatrixInvV",
				"MatrixInvP",
				"MatrixInvVP",
			};
			desc.view.vectors = {
				"CameraPos",
				"ScreenSize",
				"CameraUp",
				"CameraRight",
				"CameraFront",
			};
			return desc;
		}
	public:

		OB_RTTI();

		CameraRenderFeature(RenderScene& scene) {

		}

	private:

	};

	class CameraPass : public RenderPass {
	public:
		struct Input {
			FGResource color;
		};
		struct Output {
			FGResource color;
		};
	public:

		Output render(FG& fg, RenderView& view, Input input) const {

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

	private:
	};
}