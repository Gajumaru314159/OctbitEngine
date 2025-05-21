//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Builtin/RenderFeature/ImGuiRenderFeature.h>
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>
#include <Framework/Graphics/Builtin/RenderFeature/CameraRenderFeature.h>
//#include <Framework/Graphics/Builtin/RenderFeature/LightRenderFeature.h>
#include <Framework/Graphics/Builtin/Renderer/DefferedLightRenderer.h>
#if 1

namespace ob::graphics {

	class TestRenderPipeline {
	public:

		void render(FG& fg, RenderView& view) {

			auto earlyZ = m_earlyZ.render(fg, view, {});
			auto opaque = m_opaque.render(fg, view, { earlyZ.albedo , earlyZ.normal, earlyZ.depth });
			auto masked = m_masked.render(fg, view, { opaque.albedo , opaque.normal, earlyZ.depth });
			auto deferred = m_deferred.render(fg, view, { masked.albedo,masked.normal,earlyZ.depth });
			auto imgui = m_imgui.render(fg, view, { deferred.color });

			bool useImGui = true;

			auto camera = m_camera.render(fg, view, { useImGui ? imgui.color : deferred.color});

		}

	private:
		EarlyZPass m_earlyZ;
		OpaquePass m_opaque;
		MaskedPass m_masked;
		DeferredPass m_deferred;
		ImGuiPass m_imgui;
		CameraPass m_camera;
	};

}
#endif