//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/FrameGraph/FG.h>

#include <Framework/Graphics/Render/RenderView.h>

#include <Framework/RHI/CommandList.h>

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

		//@―---------------------------------------------------------------------------
		//! @brief      描画アイテムを追加
		//@―---------------------------------------------------------------------------
		void addRenderable(const Ref<Mesh>& mesh, const Ref<Material>& material);

		//@―---------------------------------------------------------------------------
		//! @brief      描画
		//@―---------------------------------------------------------------------------
		bool render(StringView pass,rhi::CommandList& cmdList)const;

	private:

		Map<String, Vector<Renderable>,std::less<>> m_renderablesMap;

	};
}