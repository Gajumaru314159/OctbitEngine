//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/Pipeline/UniversalRenderPipeline.h>
#include <Framework/Graphics/Render/Feature/CameraRenderFeature.h>
#include <Framework/Graphics/Render/Feature/ImGuiRenderFeature.h>
#include <Framework/Graphics/Render/Feature/MaterialRenderFeature.h>
#include <Framework/Graphics/Render/Feature/LightRenderFeature.h>
#include <Framework/Graphics/FrameGraph/FG.h>

namespace ob::graphics {

	void UniversalRenderPipeline::setup(RenderScene& scene, RenderFeatureSet& features) const {
		features.add<MaterialRenderFeature>(scene);
		features.add<ImGuiRenderFeature>(scene);
		features.add<CameraRenderFeature>(scene);
		features.add<PointLightRenderFeature>();
	}

	void UniversalRenderPipeline::render(FG& fg, RenderScene& scene) const {
		if (auto light = scene.findFeature<PointLightRenderFeature>()) {
			light->render(fg,scene);
		}
	}

	void UniversalRenderPipeline::render(FG& fg, RenderView& view) const {

		auto size = view.get<OutputViewData>().size();

		if (!size) return;

		// リソースセットアップ6

		struct Resources {
			FGResource albedo;
			FGResource normal;
			FGResource depth;
		};
		
		auto resource = fg.addPass<Resources>(
			"ResourceSetup",
			[&](FGBuilder& builder, Resources& resources) {
				rhi::RenderTextureDesc desc;
				desc.size = *size;
				{
					desc.name = "Albedo";
					desc.format = rhi::TextureFormat::RGBA8;
					desc.clear.color = Color::Black;
					resources.albedo = builder.write(builder.create(desc));
				}
				{
					desc.name = "Normal";
					desc.format = rhi::TextureFormat::RGBA8;
					desc.clear.color = Color::Normal;
					resources.normal = builder.write(builder.create(desc));
				}
				{
					desc.name = "Depth";
					desc.format = rhi::TextureFormat::D32;
					resources.depth = builder.write(builder.create(desc));
				}
			}
		);

		// TODO CullBufferを生成
		// * RenderableのBoundsをGPUに転送
		// * FrustumCullingをGPUで実行

		auto earlyZ = m_earlyZ.render(fg, view, { resource.albedo, resource.normal, resource.depth });
		auto opaque = m_opaque.render(fg, view, { earlyZ.albedo , earlyZ.normal, earlyZ.depth });
		auto masked = m_masked.render(fg, view, { opaque.albedo , opaque.normal, opaque.depth });
		auto deferred = m_deferred.render(fg, view, { masked.albedo,masked.normal,masked.depth });
		auto imgui = m_imgui.render(fg, view, { deferred.color });

		bool useImGui = true;

		auto output = m_output.render(fg, view, { useImGui ? imgui.color : deferred.color });
	}

}