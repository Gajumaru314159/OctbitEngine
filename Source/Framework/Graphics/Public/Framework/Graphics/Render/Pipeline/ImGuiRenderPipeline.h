//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/Pass/ImGuiPass.h>
#include <Framework/Graphics/Render/Pass/OutputPass.h>

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