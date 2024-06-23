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

namespace ob::graphics {

	struct GBufferData {
		FGTexture albedo;
		FGTexture normal;
		FGTexture depth;
		FGTexture uv;
	};


	class OpaqueRenderer {
	public:
		OpaqueRenderer(RenderView& view);
		bool render(FG& fg, FGBlackboard& blackboard)const;
	private:
		RenderView& m_view;
	};

	class MaskedRenderer {
	public:
		MaskedRenderer(RenderView& view);
		bool render(FG& fg, FGBlackboard& blackboard)const;
	private:
		RenderView& m_view;
	};

	class DefferedLightRenderer {
	public:
		DefferedLightRenderer(RenderView& view);
		bool render(FG& fg, FGBlackboard& blackboard, FGTexture& accumulate)const;
	private:
		RenderView& m_view;
		Ref<Material> m_material;
		Ref<Mesh> m_mesh;
	};

}