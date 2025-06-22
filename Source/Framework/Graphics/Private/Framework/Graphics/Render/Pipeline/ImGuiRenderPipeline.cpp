//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/Graphics/Render/Pipeline/ImGuiRenderPipeline.h>
#include <Framework/Graphics/Render/Feature/CameraRenderFeature.h>
#include <Framework/Graphics/Render/Feature/ImGuiRenderFeature.h>
#include <Framework/Graphics/Render/Feature/MaterialRenderFeature.h>

namespace ob::graphics {

	void ImGuiRenderPipeline::setup(RenderScene& scene, RenderFeatureSet& features) const {
		features.add<MaterialRenderFeature>(scene);
		features.add<ImGuiRenderFeature>(scene);
		features.add<CameraRenderFeature>(scene);
	}

	void ImGuiRenderPipeline::render(FG& fg, RenderView& view) const {

		auto size = view.get<OutputViewData>().size();

		if (!size) return;

		// リソースセットアップ

		struct Resources {
			FGResource color;
		};

		auto resources = fg.addPass<Resources>(
			"ResourceSetup",
			[&](FGBuilder& builder, Resources& resources) {
				rhi::RenderTextureDesc desc;
				desc.name = "Color";
				desc.size = *size;
				desc.format = rhi::TextureFormat::RGBA8;
				desc.clear.color = Color::Black;
				resources.color = builder.write(builder.create(desc));
			}
		);

		auto imgui = m_imgui.render(fg, view, { resources.color});
		auto output = m_output.render(fg, view, { imgui.color });
	}

}