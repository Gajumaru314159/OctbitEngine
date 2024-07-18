//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Mesh/Mesh.h>

namespace ob::graphics {


	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	MaterialRenderFeature::MaterialRenderFeature() {		
		m_materialId = static_cast<MaterialId>(0);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画可能なオブジェクトを追加
	//@―---------------------------------------------------------------------------
	MaterialId MaterialRenderFeature::addRenderable(const Ref<Mesh>& mesh, const Ref<Material>& material) {

		m_materialId = static_cast<MaterialId>(enum_cast(m_materialId)+1);

		for (auto [i,submesh] : Indexed(mesh->getSubMeshes())) {
			Renderable renderable;
			renderable.mesh = mesh;
			renderable.submesh = i;
			renderable.material = material;

			for (auto [name,pass] : material->getDesc().passes) {
				m_renderablesMap[name][m_materialId] = renderable;
			}
		}

		return m_materialId;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画アイテムを削除
	//@―---------------------------------------------------------------------------
	void MaterialRenderFeature::removeRenderable(MaterialId id) {
		if(id==MaterialId::Invalid)return;
		for (auto& [name, pass] : m_renderablesMap) {
			pass.erase(id);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	bool MaterialRenderFeature::render(StringView pass, rhi::CommandList& cmdList)const {

		auto itr = m_renderablesMap.find(pass);
		if (itr == m_renderablesMap.end())return false;

		auto& renderables = itr->second;

		for (auto& [maerialId,renderable] : renderables) {
			Matrix matrix;
			Ref<rhi::CommandList> cmdList2 = &cmdList;
			renderable.material->record(cmdList2, matrix, renderable.mesh, renderable.submesh, pass);
		}

		cmdList.popMarker();

		return true;
	}

}