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
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/Graphics/Mesh/MeshImpl.h>

#include <Framework/Graphics/Material/MaterialManager.h>
#include <magic_enum.hpp>

namespace ob::graphics {

	//! @brief  コンストラクタ
	MaterialImpl::MaterialImpl(const MaterialDesc& desc)
		: m_desc(desc)
	{
		MaterialBlockDesc mdesc;
		mdesc.name = desc.name;
		mdesc.textures = desc.textureProperties;
		mdesc.buffers = desc.bufferProperties;
		mdesc.matrices = desc.matrixProperties;
		mdesc.vectors = desc.colorProperties;
		mdesc.scalars = desc.floatProperties;

		m_block.construct(mdesc);

	}

	const MaterialDesc& MaterialImpl::getDesc()const {
		return m_desc;
	}

	//! @brief  プロパティがあるか
	bool MaterialImpl::hasProprty(StringView name, MaterialPropertyType type) const {
		
		return m_block->hasProprty(name, type);
	}

	//! @brief  Floatプロパティを設定
	void MaterialImpl::setFloat(StringView name, f32 value) {
		m_block->setScalar(name, value);
	}

	//! @brief  Colorプロパティを設定
	void MaterialImpl::setColor(StringView name, Color value) {
		m_block->setVector(name, value);
	}

	//! @brief  Matrixプロパティを設定
	void MaterialImpl::setMatrix(StringView name, const Matrix& value) {
		m_block->setMatrix(name, value);
	}

	//! @brief  Textureプロパティを設定
	void MaterialImpl::setTexture(StringView name, const Ref<Texture>& value) {
		m_block->setTexture(name, value,rhi::Sampler::Default());
	}

	//! @brief  Bufferプロパティを設定
	void MaterialImpl::setBuffer(StringView name, const Ref<rhi::Buffer>& value) {
		m_block->setBuffer(name, value);
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

		// グローバル変数設定
		if (auto manager = MaterialManager::Get()) {
			manager->recordGlobalShaderProperties(cmdList);
		} else {
			LOG_ERROR("MaterialManagerが未初期化です。");
		}


		m_block->record(cmdList, 0,16);

		pMesh->record(cmdList, submeshIndex);

	}

	//! @brief  
	void MaterialImpl::record(Ref<rhi::CommandList>& cmdList, Span<Matrix> matrices, const Ref<Mesh>& mesh, s32 submesh, StringView pass) {

		OB_NOTIMPLEMENTED();
	}


	//! @brief  パイプラインを生成
	Ref<rhi::PipelineState> MaterialImpl::createPipeline(StringView pass, const rhi::VertexLayout& layout,VertexLayoutId id) {

		using namespace ob::rhi;

		Ref<rhi::PipelineState> pipeline;

		// マテリアルパス取得
		auto passItr = m_desc.passes.find(pass);

		if (passItr == m_desc.passes.end()) {
			LOG_ERROR("PipelineStateの生成に失敗。{}は{}に登録されていないMaterialPassです。",pass,m_desc.name);
			return nullptr;
		}

		auto& materialPass = passItr->second;

		// 頂点レイアウト
		rhi::VertexLayout mapped;

		for (auto& attr1 : materialPass.requiredLayout) {

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
				LOG_ERROR("PipelineStateの生成に失敗。マテリアルに必要な頂点情報が足りません。");
				LOG_ERROR("Semantic:{} Type:{} Dimention:{} Index:{}", magic_enum::enum_name(attr1.semantic), magic_enum::enum_name(attr1.type), attr1.dimention, attr1.index);
				return nullptr;
			}

		}

		// RootSignature(仮)
		Ref<RootSignature> signature = [&](){

			// TODO テクスチャの複数枚対応
			RootSignatureDesc desc;
			desc.constants.set(16 * 2, 0);	// グローバルプロパティ(バッファ)
			desc.name = "Common";

			return RootSignature::Create(desc);
		}();

		// パイプライン
		{
			PipelineStateDesc desc;

			desc.name = "Material";
			desc.colors = materialPass.colors;
			desc.depth = materialPass.depth;
			//TODO RootSignatureをマテリアル内部に閉じ込める
			desc.rootSignature = signature;// MaterialManager::Get()->getSignature();// materialPass.rootSignature;
			desc.vertexLayout = layout;
			desc.vs = materialPass.vs;
			desc.ps = materialPass.ps;
			desc.blend = materialPass.blends;
			desc.rasterizer = materialPass.rasterizer;
			desc.depthStencil = materialPass.depthStencil;

			pipeline = PipelineState::Create(desc);

			if (pipeline) {
				PipelineKey key{pass,id};
				ScopeLock lock(m_lock);
				m_pipelineMap[key] = pipeline;
			}
		}


		return pipeline;

	}


}