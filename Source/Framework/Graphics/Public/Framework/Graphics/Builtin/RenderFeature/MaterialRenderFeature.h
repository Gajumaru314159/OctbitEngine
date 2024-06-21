//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/FrameGraph/FG.h>

#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {

	struct Renderable {
		Ref<Mesh>		mesh;
		s32				submesh;
		Ref<Material>	material;
		u8				flags;
		// CastShadow
		// ReveievShadow
	};


	//@―---------------------------------------------------------------------------
	//! @brief      マテリアル描画機能
	//@―---------------------------------------------------------------------------
	class MaterialRenderFeature : public RenderFeature {
	public:

		OB_RTTI();

		MaterialRenderFeature();

		void addRenderable(const Ref<Mesh>& mesh, const Ref<Material>& material);

		//@―---------------------------------------------------------------------------
		//! @brief      描画
		//@―---------------------------------------------------------------------------
		FrameGraphResource render(FG& fg, RenderView& view,String pass, FrameGraphResource targets);

	private:

		HashMap<String, Array<Renderable>> m_renderablesMap;

	};

	class MaterialRenderer {
	public:
		MaterialRenderer(RenderView& view) 
			: m_view(view)
		{

		}

		FrameGraphResource render(FG& fg, String pass, FrameGraphResource targets) {
			if (auto feature = m_view.findFeature<MaterialRenderFeature>()) {
				return feature->render(fg, m_view,pass, targets);
			}
			return {};
		}

	private:
		RenderView& m_view;
	};
}