//***********************************************************
//! @file
//! @brief		カメラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderStep.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Core/Utility/Ref.h>

#include <Framework/Graphics/Feature/MeshRenderFeature.h>
#include <Framework/Graphics/Feature/ImGuiRenderFeature.h>

namespace ob::graphics {

	class RenderPipelineAsset {
	public:

		Ref<RenderPipeline> create() {
			return nullptr;
		}
	};


	class RenderPipelineStepAsset {

	};


	class DefferedRenderPipeline : public RenderPipeline {
	public:

		DefferedRenderPipeline(RenderScene& scene)
			: RenderPipeline(scene)
		{
			RenderFeatureInjector injector;
			injector.add<MeshRenderFeature>();
			injector.add<ImGuiRenderFeature>();

		}

		UPtr<RenderView> createView() {

			RenderStepSet steps;



			getScene().visitFeatures(
				[&steps](RenderFeature& feature) {
					feature.setupView(steps);
				}
			);


			UPtr<RenderView> view = RenderView::Create();

			return view;
		}

		void render() {

			// Nativeのコマンド記録に時間がかかるため必要なデータのみ記録する
			// 描画順のソートをMainThreadにのせないためにはコマンド記録時にソート情報を持たせる必要がある

			// Compute

			// Shadow

			// GBuffer
			// Opaque指定のRendererを全取得し描画キューに追加
			// Materialで描画可能なものを制限する

			// 1RenderPass
			{
				cmd.beginRenderPass();

				if (auto materialRF = findRF<MaterialRenderFeature>()) {

					MaterialDrawParam param;
					param.tag = "Opaqie";
					param.viewMatrix = camera.matrix;
					param.sortMode = RenderSortMode::BackToFront;

					materialRF->draw(cmd, param);

				}
			}


			// GBufferLighting

			// Forward

			// Transparent

			// Filter

			// UI


		}

	};

}