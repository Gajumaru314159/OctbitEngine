//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Material/MaterialBlock.h>
#include <Framework/Graphics/Material/MaterialSystem.h>

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


	struct DrawRendelableSettings {
		String pass;
		// mask
		// sort
	};

	enum class MaterialId : u32 {
		Invalid = 0,
	};


	struct MaterialRFData {
		MaterialBlock block;
	};

	//! @brief      マテリアル描画機能
	class MaterialRenderFeature : public RenderFeature {
	public:

		OB_RTTI();

		MaterialRenderFeature(RenderScene& scene);

		//! @brief      描画アイテムを追加
		MaterialId addRenderable(const Ref<Mesh>& mesh, const Ref<Material>& material);

		//! @brief      描画アイテムを削除
		void removeRenderable(MaterialId id);

		//! @brief      描画
		bool render(Ref<rhi::CommandList>& cmdList, MaterialBlockSet& blocks, StringView pass)const;


		bool render(Ref<rhi::CommandList>& cmdList, MaterialBlockSet& blocks)const;


		MaterialBlock& getSceneBlock() { return m_sceneBlock; }

	private:

		mutable SpinLock m_lock;

		MaterialId m_materialId;
		Map<String, HashMap<MaterialId, Renderable>, std::less<>> m_renderablesMap;

		MaterialBlock m_sceneBlock;

	};

}