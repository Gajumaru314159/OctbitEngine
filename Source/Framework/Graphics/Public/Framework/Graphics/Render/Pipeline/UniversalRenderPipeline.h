//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/Pass/GBufferPass.h>
#include <Framework/Graphics/Render/Pass/DeferredPass.h>
#include <Framework/Graphics/Render/Pass/ImGuiPass.h>
#include <Framework/Graphics/Render/Pass/OutputPass.h>

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
		OutputPass m_output;
	};

}