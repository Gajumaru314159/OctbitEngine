//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>
#include <Framework/Graphics/Material/MaterialRecorder.h>
#include <Framework/Graphics/Material/MaterialSystem.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Mesh/MeshImpl.h>

namespace ob::graphics {

	MaterialRecorder::MaterialRecorder(Ref<rhi::CommandList>& cmd, MaterialRenderer& renderer)
		: m_cmdList(cmd)
	{
		m_blocks.global = &renderer.getGlobalBlock();
		m_blocks.scene = &renderer.getSceneBlock();
		m_blocks.view = &renderer.getViewBlock();
	}

	MaterialRecorder::MaterialRecorder(Ref<rhi::CommandList>& cmd, RenderScene& scene)
		: m_cmdList(cmd)
	{
		m_blocks.global = &MaterialSystem::Instance().getGlobalBlock();
		if (auto feature = scene.findFeature<MaterialRenderFeature>()) {
			m_blocks.scene = &feature->getSceneBlock();
		}
	}

	MaterialRecorder::MaterialRecorder(Ref<rhi::CommandList>& cmd)
		: m_cmdList(cmd)
	{
		m_blocks.global = &MaterialSystem::Instance().getGlobalBlock();
	}

	void MaterialRecorder::record(Ref<Mesh>& mesh, s32 submesh,Ref<Material2>& material, StringView pass) {
		material->record(m_cmdList, m_blocks, mesh, submesh, pass);
	}

}