//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Builtin/Renderer/DefferedLightRenderer.h>
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Mesh/Mesh.h>
#include <Framework/Graphics/Render/RenderPassBuilder.h>
#include <Framework/RHI/CommandList.h>

namespace ob::graphics {


	//! @brief      コンストラクタ
	MaterialRenderFeature::MaterialRenderFeature(RenderScene&) {		
		m_materialId = static_cast<MaterialId>(0);
	}

	//! @brief      描画可能なオブジェクトを追加
	MaterialId MaterialRenderFeature::addRenderable(const Ref<Mesh>& mesh, const Ref<Material>& material) {

		ScopeLock lock(m_lock);

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

	//! @brief      描画アイテムを削除
	void MaterialRenderFeature::removeRenderable(MaterialId id) {
		ScopeLock lock(m_lock);
		if(id==MaterialId::Invalid)return;
		for (auto& [name, pass] : m_renderablesMap) {
			pass.erase(id);
		}
	}

	//! @brief      描画
	bool MaterialRenderFeature::render(Ref<rhi::CommandList>& cmdList,MaterialBlockSet& blocks, StringView pass)const {

		ScopeLock lock(m_lock);
		auto itr = m_renderablesMap.find(pass);
		if (itr == m_renderablesMap.end())return false;

		auto& renderables = itr->second;

		Vector<Renderable> items;
		for (auto& [maerialId, renderable] : renderables) {
			items.push_back(renderable);
		}

		for (auto& [maerialId,renderable] : renderables) {
			renderable.material->record(cmdList,blocks ,renderable.mesh, renderable.submesh, pass);
		}

		return true;
	}

}