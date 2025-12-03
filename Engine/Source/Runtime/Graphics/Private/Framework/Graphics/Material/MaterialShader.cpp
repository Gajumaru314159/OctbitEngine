//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Material/MaterialShader.h>
#include <Framework/Graphics/Material/MaterialSystem.h>
#include <Framework/Graphics/Mesh/MeshImpl.h>
#include <Framework/Core/Thread/ScopeLock.h>
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/Types/BindlessHandle.h>
#include <Framework/RHI/RHI.h>

namespace ob::graphics {


	Ref<MaterialShader> MaterialShader::Create(const MaterialDesc& desc) {
		return new MaterialShader(desc);
	}

	MaterialShader::MaterialShader(const MaterialDesc& desc) {
		using namespace ob::rhi;

		m_desc = desc;

		bool isBindless = Device::Instance().getConfig().enableBindless;

		m_blockDesc.name = desc.name;
		m_blockDesc.textures = desc.textures;
		m_blockDesc.buffers = desc.buffers;
		m_blockDesc.matrices = desc.matrices;
		m_blockDesc.vectors = desc.vectors;
		m_blockDesc.scalars = desc.scalars;
		m_blockDesc.integers = desc.integers;
		if (!isBindless) {
			m_blockDesc.layout = MaterialBlock::CreateLayout(m_blockDesc);
		}

		auto& layouts = MaterialSystem::Instance().getLayouts();

		RootSignatureDesc rdesc;
		rdesc.name = desc.name;
		if (rhi::Device::Instance().getConfig().enableBindless) {
			rdesc.constants.set(sizeof(rhi::BindlessHandle) * 4, 0);
			rdesc.flags = RootSignatureFlag::EnableBindless;
		} else {
			rdesc.layouts = { m_blockDesc.layout, layouts.global, layouts.scene, layouts.view };
		}

		m_signature = RootSignature::Create(rdesc);
	}

	bool MaterialShader::prepare(const Ref<Mesh>& mesh) {

		auto pMesh = mesh.cast<MeshImpl>();
		if (!pMesh) return false;

		PipelineKey key;
		key.layout = pMesh->getVertexLayoutId();

		for (auto& [passName, pass] : m_desc.passes) {
			key.pass = passName;
			for (auto [qualityIndex, quality] : Indexed(pass.qualities)) {
				key.qualityIndex = qualityIndex;
				createPipeline(key, quality, pMesh->getVertexLayout());
			}
		}

		return true;
	}

	s32 MaterialShader::calcQualityIndex(StringView passName, s32 quality) const {

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

	Ref<MaterialShader::PipelineState> MaterialShader::getPipeline(const Ref<Mesh>& mesh, StringView passName, s32 quality) {

		// パス名からパスを取得
		auto passItr = m_desc.passes.find(passName);

		if (passItr == m_desc.passes.end()) {
			LOG_ERROR("PipelineStateの生成に失敗。{}は{}に登録されていないMaterialPassです。", passName, m_desc.name);
			return nullptr;
		}

		const MaterialPass& pass = passItr->second;

		// 品質選択
		s32 qualityIndex = calcQualityIndex(passName, quality);

		if (!is_in_range(qualityIndex, pass.qualities)) {
			LOG_ERROR("MaterialPassに有効なシェーダーが含まれていません [name={},quality={}]", m_desc.name, quality);
			return nullptr;
		}

		// PipelineKeyからPipelineを取得
		auto pMesh = mesh.cast<MeshImpl>();
		if (!pMesh) return nullptr;

		PipelineKey key{
			String(passName),
			qualityIndex,
			pMesh->getVertexLayoutId()
		};

		return createPipeline(key, pass.qualities.at(qualityIndex), pMesh->getVertexLayout());
	}

	//! @brief PipelineStateを作成 
	Ref<MaterialShader::PipelineState> MaterialShader::createPipeline(const PipelineKey& key, const ShaderSet& shaders, const rhi::VertexLayout& vertexLayout) {
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
