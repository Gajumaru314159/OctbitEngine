//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Builtin/RenderPipeline/UniversalRenderPipeline.h>

namespace ob::graphics {

	void UniversalRenderPipeline::setup(RenderScene& scene, RenderFeatureSet& features) const {
		features.add<MaterialRenderFeature>(scene);
		features.add<ImGuiRenderFeature>(scene);
		features.add<CameraRenderFeature>(scene);
	}

	void UniversalRenderPipeline::render(FG& fg, RenderScene& scene) const {

	}

	void UniversalRenderPipeline::render(FG& fg, RenderView& view) const {
		auto earlyZ = m_earlyZ.render(fg, view, {});
		auto opaque = m_opaque.render(fg, view, { earlyZ.albedo , earlyZ.normal, earlyZ.depth });
		auto masked = m_masked.render(fg, view, { opaque.albedo , opaque.normal, earlyZ.depth });
		auto deferred = m_deferred.render(fg, view, { masked.albedo,masked.normal,earlyZ.depth });
		auto imgui = m_imgui.render(fg, view, { deferred.color });

		bool useImGui = true;

		auto camera = m_camera.render(fg, view, { useImGui ? imgui.color : deferred.color });
	}

}