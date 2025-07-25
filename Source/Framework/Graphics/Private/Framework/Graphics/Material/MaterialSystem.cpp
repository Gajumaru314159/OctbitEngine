//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/Feature/MaterialRenderFeature.h>
#include <Framework/Graphics/Material/MaterialSystem.h>
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/PipelineState.h>
#include <Framework/Graphics/Mesh/MeshImpl.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {

	static MaterialBlockDesc Convert(StringView name, const MaterialPropertiesDesc& desc) {
		MaterialBlockDesc blockDesc;
		blockDesc.name = name;
		blockDesc.textures = desc.textures;
		blockDesc.buffers = desc.buffers;
		blockDesc.matrices = desc.matrices;
		blockDesc.vectors = desc.vectors;
		blockDesc.scalars = desc.scalars;
		blockDesc.integers = desc.integers;
		return blockDesc;
	}

	MaterialSystem::MaterialSystem(const MaterialSystemDesc& desc) {

		m_descs.global = Convert("MaterialGlobal", desc.properties.global);
		m_descs.scene = Convert("MaterialScene", desc.properties.scene);
		m_descs.view = Convert("MaterialView", desc.properties.view);
		if (!rhi::Device::Instance().getConfig().enableBindless) {
			m_descs.global.layout = m_layouts.global = MaterialBlock::CreateLayout(m_descs.global);
			m_descs.scene.layout = m_layouts.scene = MaterialBlock::CreateLayout(m_descs.scene);
			m_descs.view.layout = m_layouts.view = MaterialBlock::CreateLayout(m_descs.view);
		}

		m_globalBlock = MaterialBlock(m_descs.global);
	}

	MaterialBlock MaterialSystem::createSceneBlock(StringView name) const {
		MaterialBlockDesc desc = m_descs.scene;
		desc.name = name;
		return MaterialBlock(desc);
	}

	MaterialBlock MaterialSystem::createViewBlock(StringView name) const {
		MaterialBlockDesc desc = m_descs.scene;
		desc.name = name;
		return MaterialBlock(desc);
	}

	MaterialBlockSet::MaterialBlockSet()
	{
		global = &MaterialSystem::Instance().getGlobalBlock();
	}

	MaterialBlockSet::MaterialBlockSet(RenderView& view)
	{
		global = &MaterialSystem::Instance().getGlobalBlock();
		if (auto feature = view.findFeature<MaterialRenderFeature>()) {
			scene = &feature->getSceneBlock();
		}
		if (view.contains<MaterialRFData>()) {
			this->view = &view.get<MaterialRFData>().block;
		}
	}

	MaterialBlockSet::MaterialBlockSet(RenderScene& scene)
	{
		global = &MaterialSystem::Instance().getGlobalBlock();
		if (auto feature = scene.findFeature<MaterialRenderFeature>()) {
			this->scene = &feature->getSceneBlock();
		}
	}


}