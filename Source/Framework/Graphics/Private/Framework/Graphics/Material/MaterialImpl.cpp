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

	//! @brief  コンストラクタ
	MaterialImpl::MaterialImpl(const MaterialDesc& desc)
		: m_desc(desc)
	{
		using namespace ob::rhi;

		MaterialBlockDesc bdesc;
		bdesc.name = desc.name;
		bdesc.textures = desc.textures;
		bdesc.buffers = desc.buffers;
		bdesc.matrices = desc.matrices;
		bdesc.vectors = desc.colors;
		bdesc.scalars = desc.scalars;
		m_block = MaterialBlock(bdesc);

		RootSignatureDesc rdesc;
		rdesc.name = desc.name;
		rdesc.layouts = {
			m_block.getLayout(),
			MaterialSystem::Instance().getLayouts().global,
			MaterialSystem::Instance().getLayouts().scene,
			MaterialSystem::Instance().getLayouts().view,
		};
		rdesc.flags = RootSignatureFlag::EnableBindless;
		rdesc.constants.set(sizeof(BindlessHandle) * 4, 0);
		m_signature = RootSignature::Create(rdesc);
	}

	const MaterialDesc& MaterialImpl::getDesc()const {
		return m_desc;
	}

	//! @brief  プロパティがあるか
	bool MaterialImpl::hasProprty(StringView name, MaterialPropertyType type) const {
		return m_block.hasProperty(name, type);
	}

	//! @brief  Floatプロパティを設定
	void MaterialImpl::setFloat(StringView name, f32 value) {
		m_block.setScalar(name, value);
	}

	//! @brief  Colorプロパティを設定
	void MaterialImpl::setColor(StringView name, Color value) {
		m_block.setVector(name, value);
	}

	//! @brief  Matrixプロパティを設定
	void MaterialImpl::setMatrix(StringView name, const Matrix& value) {
		m_block.setMatrix(name, value);
	}

	//! @brief  Textureプロパティを設定
	void MaterialImpl::setTexture(StringView name, const Ref<Texture>& value) {
		m_block.setTexture(name, value,rhi::Sampler::Default());
	}

	//! @brief  Bufferプロパティを設定
	void MaterialImpl::setBuffer(StringView name, const Ref<rhi::Buffer>& value) {
		m_block.setBuffer(name, value);
	}

	//! @brief  GPUリソースの事前生成
	bool MaterialImpl::reserve(const Ref<Mesh>& mesh) {

		auto pMesh = mesh.cast<MeshImpl>();
		if (!pMesh) return false;

		auto& layout = mesh->getVertexLayout();
		for (auto& [name,pass] : m_desc.passes) {
			createPipeline(name, layout, pMesh->getVertexLayoutId());
		}

		return true;
	}

	//! @brief  描画コマンドを記録
	void MaterialImpl::record(Ref<rhi::CommandList>& cmdList, const Matrix& matrix, const Ref<Mesh>& mesh, s32 submeshIndex, StringView pass) {
		// 1. 定数バッファのデスクリプタ設定
		// 2. テクスチャのデスクリプタ設定
		// 3. サンプラーのデスクリプタ設定
		// 4. バッファのデスクリプタ設定

		// カメラ情報 → RenderSystemやModelで設定する
		// 複数パス → パスを引数に取る
		// シェーダ設定 → 
		// メッシュの描画 → パイプラインごとに頂点レイアウトが違う

		auto pMesh = mesh.cast<MeshImpl>();
		if (!pMesh) return;

		auto submesh = pMesh->getSubMesh(submeshIndex);
		if (submesh.indexCount <= 0) return;


		Ref<rhi::PipelineState> pipeline;

		PipelineKey key{
			pass,
			pMesh->getVertexLayoutId()
		};

		{
			ScopeLock lock(m_lock);
			if (auto found = m_pipelineMap.find(key); found != m_pipelineMap.end()) {
				pipeline = found->second;
			}
		}
		if(!pipeline) {
			pipeline = createPipeline(pass, pMesh->getVertexLayout(),pMesh->getVertexLayoutId());
		}

		if (!pipeline)
			return;

		cmdList->setPipelineState(pipeline);

		// TODO 異なるスコープのMaterialBlockを再バインドする必要があるか未確認

		m_block.record(cmdList, 0);

		MaterialManager::Instance().recordGlobalShaderProperties(cmdList);

		pMesh->record(cmdList, submeshIndex);

	}

	//! @brief  
	void MaterialImpl::record(Ref<rhi::CommandList>& cmdList, Span<Matrix> matrices, const Ref<Mesh>& mesh, s32 submesh, StringView pass) {

		OB_NOTIMPLEMENTED();
	}


	//! @brief  パイプラインを生成
	Ref<rhi::PipelineState> MaterialImpl::createPipeline(StringView passName, const rhi::VertexLayout& layout,VertexLayoutId id) {

		using namespace ob::rhi;

		Ref<rhi::PipelineState> pipeline;

		// マテリアルパス取得
		auto passItr = m_desc.passes.find(passName);

		if (passItr == m_desc.passes.end()) {
			LOG_ERROR("PipelineStateの生成に失敗。{}は{}に登録されていないMaterialPassです。",passName,m_desc.name);
			return nullptr;
		}

		auto& pass = passItr->second;

		// 頂点レイアウト

		ShaderKeywordSet keywords = pass.keywords;

		// TODO LODレベルに応じたシェーダの選択
		auto shaderSetItr = m_desc.shaders.find(keywords);
		if (shaderSetItr == m_desc.shaders.end()) {
			LOG_ERROR("PipelineStateの生成に失敗。登録されていないShaderSetです。");
			return nullptr;
		}

		auto& shaderSet = shaderSetItr->second;

		// 選択したシェーダーに必要な頂点情報があるかを確認し、対応マップを作成する。
		rhi::VertexLayout mapped;
		Vector<InputLayout> missingLayouts;
		for (auto& attr1 : shaderSet.inputLayout) {
			bool ok = false;
			for (auto& attr2 : layout.attributes) {

				if (
					attr1.semantic == attr2.semantic &&
					attr1.type == attr2.type &&
					attr1.dimention == attr2.dimention &&
					attr1.index == attr2.index
					)
				{
					mapped.attributes.push_back(attr2);
					ok = true;
					break;
				}
			}
			if (ok == false) {
				missingLayouts.push_back(attr1);
			}
		}
		if (!missingLayouts.empty()) {
			String message = Format("PipelineStateの生成に失敗。マテリアルに必要な頂点情報が足りません。 [name={}]",m_desc.name);
			for (auto& layout : missingLayouts) {
				message += Format("\n* Semantic:{} Type:{} Dimention:{} Index:{}", magic_enum::enum_name(layout.semantic), magic_enum::enum_name(layout.type), layout.dimention, layout.index);
			}
			LOG_ERROR("{}", message);
			return nullptr;
		}

		// RootSignature(仮)
		Ref<RootSignature> signature = [&](){

			RootSignatureDesc desc;
			desc.name = m_desc.name;

			if (RHI::Instance().getConfig().enableBindless) {
				// TODO マジックナンバーを共通ヘッダーに定義
				desc.constants.set(sizeof(BindlessHandle) * 4, 0);
				desc.flags |= RootSignatureFlag::EnableBindless;
			} else {
				auto& layouts = MaterialSystem::Instance().getLayouts();
				desc.layouts = {
					m_materialLayout,
					layouts.global,
					layouts.scene,
					layouts.view,
				};
			}			

			return RootSignature::Create(desc);
		}();

		// パイプライン
		{
			PipelineStateDesc desc;

			desc.name = m_desc.name;
			desc.colors = shaderSet.colors;
			desc.depth = shaderSet.depth;
			desc.rootSignature = signature;
			desc.vertexLayout = layout;
			desc.vs = shaderSet.vs;
			desc.ps = shaderSet.ps;
			desc.blend = shaderSet.blends;
			desc.rasterizer = shaderSet.rasterizer;
			desc.depthStencil = shaderSet.depthStencil;

			pipeline = PipelineState::Create(desc);

			if (pipeline) {
				PipelineKey key{passName,id};
				ScopeLock lock(m_lock);
				m_pipelineMap[key] = pipeline;
			}
		}

		return pipeline;

	}


}