//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Material/MaterialImpl.h>
#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Sampler.h>
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/Graphics/Mesh/MeshImpl.h>
#include <Framework/RHI/RHI.h>

#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/Graphics/Material/MaterialSystem.h>
#include <magic_enum.hpp>

namespace ob::graphics {

	MaterialImpl::MaterialImpl(const MaterialDesc& desc) {
		using namespace ob::rhi;

		m_desc = desc;

		// NOTE コンストラクタでMaterialBlockを作成する = GameThreadで作成するとヒッチの原因となる可能性がある。許容するか、非同期生成するかは要検討。
		// if(Graphics::IsGameThread()) LOG_WARNING("GameThreadでマテリアルが生成されています。ヒッチを避けるために非同期スレッドで生成してください。");

		MaterialBlockDesc bdesc;
		bdesc.name = desc.name;
		bdesc.textures = desc.textures;
		bdesc.buffers = desc.buffers;
		bdesc.matrices = desc.matrices;
		bdesc.vectors = desc.vectors;
		bdesc.scalars = desc.scalars;
		bdesc.integers = desc.integers;
		m_block = MaterialBlock(bdesc);

		auto& layouts = MaterialSystem::Instance().getLayouts();

		RootSignatureDesc rdesc;
		rdesc.name = desc.name;
		if (rhi::RHI::Instance().getConfig().enableBindless) {
			rdesc.constants.set(sizeof(rhi::BindlessHandle) * 4, 0);
		} else {
			rdesc.layouts = { m_block.getLayout(), layouts.global, layouts.scene, layouts.view };
		}
		rdesc.flags = RootSignatureFlag::EnableBindless;

		m_signature = RootSignature::Create(rdesc);
	}

	bool MaterialImpl::prepare(const Ref<Mesh>& mesh) {

		auto pMesh = mesh.cast<MeshImpl>();
		if (!pMesh) return false;

		// NOTE ShaderLODやVariantなど、どこまで事前生成するかという問題がある。

		PipelineKey key;
		key.layout = pMesh->getVertexLayoutId();

		auto& layout = mesh->getVertexLayout();
		for (auto& [passName, pass] : m_desc.passes) {
			key.pass = passName;
			for (auto [qualityIndex, quality] : Indexed(pass.qualities)) {
				key.qualityIndex = qualityIndex;
				createPipeline(key, quality, pMesh->getVertexLayout());
			}
		}

		return true;
	}

	s32 MaterialImpl::calcQualityIndex(StringView passName, s32 quality) const {

		// パス名か定義を取得
		auto passItr = m_desc.passes.find(passName);

		if (passItr == m_desc.passes.end()) {
			LOG_ERROR("PipelineStateの生成に失敗。{}は{}に登録されていないMaterialPassです。", passName, m_desc.name);
			return -1;
		}

		const MaterialPass& pass = passItr->second;

		if (pass.qualities.empty()) {
			LOG_ERROR("MaterialPassに有効なシェーダーが含まれていません [name={}]", m_desc.name);
			return -1;
		}

		// 品質選択
		s32 index = -1;
		for (auto& shaders : pass.qualities) {
			if (quality < shaders.quality) break;
			index++;
		}

		return index;
	}

	void MaterialImpl::record(Ref<rhi::CommandList>& commandList, MaterialBlockSet& blocks, const Ref<Mesh>& mesh, s32 submesh, StringView passName, s32 quality) {

		// パス名からパスを取得
		auto passItr = m_desc.passes.find(passName);

		if (passItr == m_desc.passes.end()) {
			LOG_ERROR("PipelineStateの生成に失敗。{}は{}に登録されていないMaterialPassです。", passName, m_desc.name);
			return;
		}

		const MaterialPass& pass = passItr->second;

		// 品質選択
		s32 qualityIndex = calcQualityIndex(passName, quality);

		if (!is_in_range(qualityIndex, pass.qualities)) {
			LOG_ERROR("MaterialPassに有効なシェーダーが含まれていません [name={},quality={}]", m_desc.name, quality);
			return;
		}

		// PipelineKeyからPipelineを取得
		auto pMesh = mesh.cast<MeshImpl>();
		if (!pMesh) return;

		PipelineKey key{
			String(passName),
			qualityIndex,
			pMesh->getVertexLayoutId()
		};

		Ref<rhi::PipelineState> pipeline = createPipeline(key, pass.qualities.at(qualityIndex), pMesh->getVertexLayout());
		OB_ASSERT_EXPR(pipeline);

		commandList->setPipelineState(pipeline);

		m_block.record(commandList, 0);
		if (blocks.global)blocks.global->record(commandList, 1);
		if (blocks.scene)blocks.scene->record(commandList, 2);
		if (blocks.view)blocks.view->record(commandList, 3);

		pMesh->record(commandList, submesh);
	}

	//! @brief PipelineStateを作成 
	Ref<rhi::PipelineState> MaterialImpl::createPipeline(const PipelineKey& key, const ShaderSet& shaders, const rhi::VertexLayout& vertexLayout) {
		using namespace ob::rhi;

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