//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Material/MaterialSystem.h>
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/PipelineState.h>
#include <Framework/Graphics/Mesh/MeshImpl.h>

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
		m_layouts.global = MaterialBlock::CreateLayout(m_descs.global,1);
		m_layouts.scene = MaterialBlock::CreateLayout(m_descs.scene,2);
		m_layouts.view = MaterialBlock::CreateLayout(m_descs.view,3);

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

	//----------------------------------------------------------------
	Ref<Material2> Material2::Create(const MaterialDesc& desc) {
		return new Material2(desc);
	}

	Material2::Material2(const MaterialDesc& desc) {
		using namespace ob::rhi;

		m_desc = desc;

		// NOTE コンストラクタでMaterialBlockを作成する = GameThreadで作成するとヒッチの原因となる可能性がある。許容するか、非同期生成するかは要検討。

		MaterialBlockDesc bdesc;
		bdesc.name = desc.name;
		bdesc.textures = desc.textures;
		bdesc.scalars = desc.scalars;
		m_block = MaterialBlock(bdesc);

		auto& layouts = MaterialSystem::Instance().getLayouts();

		RootSignatureDesc rdesc;
		rdesc.name = desc.name;
		rdesc.layouts = { m_block.getLayout(), layouts.global, layouts.scene, layouts.view };

		m_signature = RootSignature::Create(rdesc);
	}

	bool Material2::prepare(const Ref<Mesh>& mesh) {

		auto pMesh = mesh.cast<MeshImpl>();
		if (!pMesh) return false;

		// NOTE ShaderLODやVariantなど、どこまで事前生成するかという問題がある。

		auto& layout = mesh->getVertexLayout();
		for (auto& [name, pass] : m_desc.passes) {
			for (auto& quality : pass.qualities) {
				createPipeline(name, quality.quality,pMesh->getVertexLayout(), pMesh->getVertexLayoutId());
			}
		}

		return true;
	}

	void Material2::record(Ref<rhi::CommandList>& commandList, MaterialBlockSet& blocks, const Ref<Mesh>& mesh, s32 submesh, StringView passName, s32 quality) {

		// パス名か定義を取得
		auto passItr = m_desc.passes.find(passName);

		if (passItr == m_desc.passes.end()) {
			LOG_ERROR("PipelineStateの生成に失敗。{}は{}に登録されていないMaterialPassです。", passName, m_desc.name);
			return;
		}

		const MaterialPass& pass = passItr->second;

		// キーワードからシェーダーセットを取得
		s32 qualityIndex = 0;
		for (auto& shaders : pass.qualities) {
			if (shaders.quality >= quality) break;
			qualityIndex++;
		}

		auto pMesh = mesh.cast<MeshImpl>();
		if (!pMesh) return;

		PipelineKey key{
			String(passName),
			qualityIndex,
			pMesh->getVertexLayoutId()
		};

		Ref<rhi::PipelineState> pipeline;
		{
			ScopeLock lock(m_pipelinesLock);
			pipeline = m_pipelines.at(key);
		}

		commandList->setPipelineState(pipeline);

		m_block.record(commandList, 0);
		if (blocks.global)blocks.global->record(commandList, 1);
		if (blocks.scene)blocks.scene->record(commandList, 2);
		if(blocks.view)blocks.view->record(commandList, 3);

		pMesh->record(commandList, submesh);
	}

	//! @brief PipelineStateを作成 
	Ref<rhi::PipelineState> Material2::createPipeline(StringView passName, s32 quality, const rhi::VertexLayout& vertexLayout, VertexLayoutId id) {
		using namespace ob::rhi;

		// パス名か定義を取得
		auto passItr = m_desc.passes.find(passName);

		if (passItr == m_desc.passes.end()) {
			LOG_ERROR("PipelineStateの生成に失敗。{}は{}に登録されていないMaterialPassです。", passName, m_desc.name);
			return nullptr;
		}

		const MaterialPass& pass = passItr->second;

		// キーワードからシェーダーセットを取得
		s32 qualityIndex = 0;
		for (auto& shaders : pass.qualities) {
			if (shaders.quality >= quality) break;
			qualityIndex++;
		}

		const ShaderSet& shaders = pass.qualities.at(qualityIndex);

		PipelineKey key{
			String(passName),
			qualityIndex,
			id
		};

		// 既に作成済み
		{
			ScopeLock lock(m_pipelinesLock);
			if (auto found = m_pipelines.find(key); found != m_pipelines.end()) {
				return found->second;
			}
		}


		// PipelineStateを作成
		PipelineStateDesc desc;
		desc.name = m_desc.name;
		desc.colors = shaders.colors;
		desc.depth = shaders.depth;
		desc.rootSignature = m_signature;
		desc.vertexLayout = vertexLayout;
		desc.vs = shaders.vs;
		desc.ps = shaders.ps;
		desc.sample = shaders.sample;
		desc.blend = shaders.blend;
		desc.rasterizer = shaders.rasterizer;
		desc.depthStencil = shaders.depthStencil;

		Ref<PipelineState> pipeline = PipelineState::Create(desc);

		// マップに登録
		{
			ScopeLock lock(m_pipelinesLock);
			m_pipelines[key] = pipeline;
		}

		return pipeline;
	}

}