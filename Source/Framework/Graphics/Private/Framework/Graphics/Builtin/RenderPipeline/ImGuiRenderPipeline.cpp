//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Builtin/RenderPipeline/ImGuiRenderPipeline.h>

namespace ob::graphics {

	void ImGuiRenderPipeline::setup(RenderScene& scene, RenderFeatureSet& features) const {
		features.add<MaterialRenderFeature>(scene);
		features.add<ImGuiRenderFeature>(scene);
		features.add<CameraRenderFeature>(scene);
	}

	void ImGuiRenderPipeline::render(FG& fg, RenderView& view) const {
		auto imgui = m_imgui.render(fg, view, {});
		auto camera = m_camera.render(fg, view, { imgui.color });
	}

}