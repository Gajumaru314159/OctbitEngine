//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Builtin/RenderFeature/ImGuiRenderFeature.h>
#include <Framework/Graphics/Builtin/RenderFeature/CameraRenderFeature.h>

namespace ob::graphics {

	class ImGuiRenderPipeline : public RenderPipeline {
	public:
		static Ref<ImGuiRenderPipeline> Create() {
			return new ImGuiRenderPipeline();
		}
	private:
		ImGuiRenderPipeline() {}
	public:
		void setup(RenderScene& scene, RenderFeatureSet& features) const override;
		void render(FG& fg, RenderView& view) const override;
	private:
		ImGuiPass m_imgui;
		CameraPass m_camera;
	};

}