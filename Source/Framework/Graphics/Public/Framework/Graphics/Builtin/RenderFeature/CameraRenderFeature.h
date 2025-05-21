//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/SwapChain.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderPass.h>
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>
#include <Framework/Graphics/Material/MaterialPropertiesDesc.h>

namespace ob::graphics {

	class CameraPass;

	struct CameraRFData {
		Ref<rhi::RenderTexture> output;				//!< 描画先
		Rect                    rect;				//!< 描画範囲の矩形
		Viewport				viewport;			//!< 描画範囲

		Ref<rhi::SwapChain>		swapChain;			//!< 

		void setSwapChain(Ref<rhi::RenderTexture>& output) {

		}
		void setSwapChain(Ref<rhi::SwapChain>& swapChain) {
			this->swapChain = swapChain;
			onSwapChainUpdated();
			swapChain->addEventListener(hSwapChainUpdated, { *this,&CameraRFData::onSwapChainUpdated });
		}
	private:
		rhi::SwapChainEventHandle    hSwapChainUpdated;	//!< 

		void onSwapChainUpdated() {
			rhi::RenderTextureDesc desc;
			desc.name = Format("SwapChain_{}", swapChain->getName());
			desc.size = swapChain->getDesc().size;
			desc.format = rhi::TextureFormat::RGBA8;
			desc.clear.color = Color::Black;
			output = rhi::RenderTexture::Create(desc);
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

		//! @brief MaterialRenderFeature の描画パスをセットアップします。
		void setupPasses(RenderPassBuilder& builder) const override {
			builder.add<CameraPass>();
		}


		//! @brief レンダービューのセットアップする
		void setup(RenderView& view) override {
			auto& camera = view.get<CameraRFData>();
		}

	private:

	};

	class CameraPass : public RenderPass {
	public:
		struct Input {
			FGResource color;

			void connect(FGConnections& connections) {
				color = connections.get("CameraPass.color");
			}
		};
		struct Output {
			FGResource color;

			void connect(FGConnections& connections) {
				connections.set("CameraPass.color", color);
			}
		};
	public:

		Output render(FG& fg, RenderView& view, Input input) const {

			auto& camera = view.get<CameraRFData>();
			auto& material = view.get<MaterialRFData>();
			
			if (!camera.output) return {input.color};

			material.block.setMatrix("MatrixV",Matrix::Identity);

			return fg.addPass<Output>(
				"CameraPass",
				[&](FGBuilder& builder, Output& output) {
					output.color = builder.read(input.color);
					builder.setSideEffect();
				},
				[&](const Output& output,FGResources& resources, Ref<rhi::CommandList>& cmdList) {
					auto albedo = resources.getTexture(output.color);

					auto& camera = view.get<CameraRFData>();

					cmdList->applySwapChain(camera.swapChain, albedo);

				}
			);

		}

	private:
	};
}