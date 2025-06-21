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
#include <Framework/Graphics/Builtin/Renderer/DefferedLightRenderer.h>

namespace ob::graphics {

	class UniversalRenderPipeline : public RenderPipeline {
	public:
		static Ref<UniversalRenderPipeline> Create() {
			return new UniversalRenderPipeline();
		}
	private:
		UniversalRenderPipeline() {}
	public:
		void setup(RenderScene& scene, RenderFeatureSet& features) const override;
		void render(FG& fg, RenderScene& scene) const override;
		void render(FG& fg, RenderView& view) const override;
	private:
		// Global;

		// Local
		EarlyZPass m_earlyZ;
		OpaquePass m_opaque;
		MaskedPass m_masked;
		DeferredPass m_deferred;
		ImGuiPass m_imgui;
		CameraPass m_camera;
	};

}