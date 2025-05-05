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


	class EarlyZRenderer {
	public:
		EarlyZRenderer(RenderView& view, MaterialRenderer& renderer);
		bool render(FG& fg, FGBlackboard& blackboard)const;
	private:
		RenderView& m_view;
		MaterialRenderer& m_materialRenderer;
	};

	class OpaqueRenderer {
	public:
		OpaqueRenderer(RenderView& view, MaterialRenderer& renderer);
		bool render(FG& fg, FGBlackboard& blackboard)const;
	private:
		RenderView& m_view;
		MaterialRenderer& m_materialRenderer;
	};

	class MaskedRenderer {
	public:
		MaskedRenderer(RenderView& view, MaterialRenderer& renderer);
		bool render(FG& fg, FGBlackboard& blackboard)const;
	private:
		RenderView& m_view;
		MaterialRenderer& m_materialRenderer;
	};

	class DefferedLightRenderer {
	public:
		DefferedLightRenderer(RenderView& view, MaterialRenderer& material);
		bool render(FG& fg, FGBlackboard& blackboard, FGTexture& accumulate)const;

		void setDebugMode(s32 mode);
	private:
		RenderView& m_view;
		MaterialRenderer& m_materialRenderer;
		Ref<Material> m_material;
		Ref<Mesh> m_mesh;
	};

}