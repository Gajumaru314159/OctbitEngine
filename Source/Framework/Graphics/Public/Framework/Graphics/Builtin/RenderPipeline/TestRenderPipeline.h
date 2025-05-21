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
#if 0

namespace ob::graphics {



	class TestRenderPipeline {
	public:
		OB_RTTI();
		TestRenderPipeline(RenderView& view);

		FGTexture render(FG& fg, RenderView& view) {

			// auto earlyZ = m_earlyZ.render(fg, view, {});
			// auto opaque = m_opaque.render(fg, view, { earlyZ.albedo , earlyZ.normal, earlyZ.depth});
			// auto masked = m_masked.render(fg, view, {});

		}


	private:

		template<class T>
		void setup(RenderView& view) {
			if (auto feature = m_view.findFeature<T>()) {
				feature->setup(view);
			}
			else {
				LOG_ERROR("{}はRenderSceneに登録されていません", Type::Get<T>().name());
			}
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