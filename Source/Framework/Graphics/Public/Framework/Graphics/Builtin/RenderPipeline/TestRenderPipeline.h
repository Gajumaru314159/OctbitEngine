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


	class GlobalTestPass : public RenderPass {
	public:
		struct Input {
			void connect(FGConnections& connections) {
			}
		};
		struct Output {
			void connect(FGConnections& connections) {
			}
		};
	public:
		GlobalTestPass() {}
		Output render(FG& fg, Input input)const {
		}
	};

	class UpscalePass : public RenderPass {
	public:
		struct Input {
			void connect(FGConnections& connections) {
			}
		};
		struct Output {
			void connect(FGConnections& connections) {}
		};
		struct Param {
			f32 scale = 1.2f;
		};
		Output render(FG& fg, RenderView& view,Input input) const {
			auto& data = view.get<MaterialRFData>();
			data.block.setScalar("g_upsacale",1.2);
		}
	};

	struct TestRenderPipelineConfig {
		bool useImGui = true;
	};

	class TestRenderPipeline : public IRenderPipeline {
	public:
		void render(FG& fg, RenderViewList& views) override {

			auto global = m_global.render(fg, {});

			for (auto& view : views) {

				auto viewData = view.get<RenderViewData>();

				if (viewData.pipeline == "PBR") {
					renderPBR(fg, view);
				}
				if (viewData.pipeline == "Tool") {
					renderOnlyTool(fg, view);
				}
			}
		}

	private:

		void renderPBR(FG& fg, RenderView& view) {
			auto earlyZ = m_earlyZ.render(fg, view, {});
			auto opaque = m_opaque.render(fg, view, { earlyZ.albedo , earlyZ.normal, earlyZ.depth });
			auto masked = m_masked.render(fg, view, { opaque.albedo , opaque.normal, earlyZ.depth });
			auto deferred = m_deferred.render(fg, view, { masked.albedo,masked.normal,earlyZ.depth });
			auto imgui = m_imgui.render(fg, view, { deferred.color });

			bool useImGui = m_config->useImGui;

			auto camera = m_camera.render(fg, view, { useImGui ? imgui.color : deferred.color });
		}

		void renderOnlyTool(FG& fg, RenderView& view) {
			auto earlyZ = m_earlyZ.render(fg, view, {});
			auto imgui = m_imgui.render(fg, view, { earlyZ.albedo });
			auto camera = m_camera.render(fg, view, { imgui.color });
		}

	private:

		SPtr<TestRenderPipelineConfig> m_config;

		// Global
		GlobalTestPass m_global;

		// Local
		EarlyZPass m_earlyZ;
		OpaquePass m_opaque;
		MaskedPass m_masked;
		DeferredPass m_deferred;
		ImGuiPass m_imgui;
		CameraPass m_camera;
	};

}
#endif