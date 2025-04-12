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
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/Graphics/Mesh/MeshImpl.h>

#include <Framework/Graphics/Material/MaterialManager.h>
#include <magic_enum.hpp>

namespace ob::graphics {

	//! @brief  コンストラクタ
	MaterialImpl::MaterialImpl(const MaterialDesc& desc)
		: m_desc(desc)
	{
		using namespace ob::rhi;

		// バッファの配列は128bitアラインメント
		auto arrayAlignment = 128;

		// プロパティ名とバッファ対応
		s32 bufferSize = 0;	
		for (auto& name : desc.floatProperties) {
			auto [itr,added] = m_propertyMap.try_emplace(name, ValuePropertyDesc{ PropertyType::Float,bufferSize });
			if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。",name); return; }
			bufferSize += sizeof(f32);
		}
		bufferSize += sizeof(f32) * (8-desc.floatProperties.size()); // TEST
		bufferSize = align_up(bufferSize, arrayAlignment);
		for (auto& name : desc.colorProperties) {
			auto [itr, added] = m_propertyMap.try_emplace(name, ValuePropertyDesc{ PropertyType::Color,bufferSize });
			if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
			bufferSize += sizeof(Color);
		}
		bufferSize += sizeof(Color) * (8 - desc.colorProperties.size()); // TEST
		bufferSize = align_up(bufferSize, arrayAlignment);
		for (auto& name : desc.matrixProperties) {
			auto [itr, added] = m_propertyMap.try_emplace(name, ValuePropertyDesc{ PropertyType::Matrix,bufferSize });
			if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
			bufferSize += sizeof(Matrix);
		}
		bufferSize += sizeof(Matrix) * (8 - desc.matrixProperties.size()); // TEST
		bufferSize = align_up(bufferSize, arrayAlignment);

		// プロパティ名とテクスチャ番号対応
		for (auto [index,name] : Indexed(desc.textureProperties)) {
			auto [itr, added] = m_propertyMap.try_emplace(name, ValuePropertyDesc{ PropertyType::Texture,(s32)index});
			if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
		}
		m_textures.resize(desc.textureProperties.size());

		// プロパティ名とテクスチャ番号対応
		for (auto [index, name] : Indexed(desc.bufferProperties)) {
			auto [itr, added] = m_propertyMap.try_emplace(name, ValuePropertyDesc{ PropertyType::Buffer,(s32)index });
			if (!added) { LOG_ERROR("プロパティ[{}]はマテリアルに既に含まれています。", name); return; }
		}
		m_buffers.resize(desc.bufferProperties.size());

		// バッファ生成
		if (0 < bufferSize) {
			auto bufferDesc = rhi::BufferDesc::Constant(bufferSize);
			bufferDesc.name = Format("MaterialParameter ({})",desc.name);
			m_buffer = rhi::Buffer::Create(bufferDesc);
			OB_ASSERT_EXPR(m_buffer);
			m_bufferBlob.resize(bufferSize);
			memset(m_bufferBlob.data(), 0, m_bufferBlob.size());
		}

		// テーブル生成(バッファ)
		if (0 < bufferSize) {
			BindingSlot slot { Binding::ConstantBuffer(0) };
			m_dynamicTable = rhi::DescriptorTable::Create(slot);
			OB_ASSERT_EXPR(m_dynamicTable);
			m_dynamicTable->setResource(0, m_buffer);
		}

		// テーブル生成(テクスチャ)
		if (desc.textureProperties.size()) {
			BindingSlot slot0;
			BindingSlot slot1;

			for (auto& name : desc.textureProperties) {
				slot0.items.push_back(Binding::Texture());
			}
			for (auto& name : desc.textureProperties) {
				slot1.items.push_back(Binding::Texture());
			}

			m_textureTable = rhi::DescriptorTable::Create(slot0);
			m_samplerTable = rhi::DescriptorTable::Create(slot1);
			OB_ASSERT_EXPR(m_textureTable);
			OB_ASSERT_EXPR(m_samplerTable);
		}
		// テーブル生成(バッファ)
		if (desc.bufferProperties.size()) {
			BindingSlot slot;
			for (auto& name : desc.bufferProperties) {
				slot.items.push_back(Binding::ConstantBuffer());
			}
			m_bufferTable = rhi::DescriptorTable::Create(slot);
			OB_ASSERT_EXPR(m_bufferTable);
		}

	}

	const MaterialDesc& MaterialImpl::getDesc()const {
		return m_desc;
	}

	//! @brief  プロパティがあるか
	bool MaterialImpl::hasProprty(StringView name, PropertyType type) const {
		if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {
			return found->second.type == type;
		}
		return false;
	}

	//! @brief  Floatプロパティを設定
	void MaterialImpl::setFloat(StringView name, f32 value) {
		setValueProprty(name, PropertyType::Float, value);
	}

	//! @brief  Colorプロパティを設定
	void MaterialImpl::setColor(StringView name, Color value) {
		setValueProprty(name, PropertyType::Color, value);
	}

	//! @brief  Matrixプロパティを設定
	void MaterialImpl::setMatrix(StringView name, const Matrix& value) {
		setValueProprty(name, PropertyType::Matrix,
#if 1
			value
#else
			value.transposed()
#endif
		);
	}

	//! @brief  Textureプロパティを設定
	void MaterialImpl::setTexture(StringView name, const Ref<Texture>& value) {
		if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {

			auto& desc = found->second;
			if (desc.type != PropertyType::Texture)return;
			if (!is_in_range(desc.offset, m_textures))return;

			m_textures[desc.offset] = value;

			m_textureTable->setResource(desc.offset, value);

		}
	}

	//! @brief  Bufferプロパティを設定
	void MaterialImpl::setBuffer(StringView name, const Ref<rhi::Buffer>& value) {
		if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {

			auto& desc = found->second;
			if (desc.type != PropertyType::Buffer)return;
			if (!is_in_range(desc.offset, m_buffers))return;

			m_buffers[desc.offset] = value;

			m_bufferTable->setResource(desc.offset, value);

		}
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


		// TODO 複数パスある場合は余計なので別途更新関数を回す
		m_buffer->update(m_bufferBlob.size(), m_bufferBlob.data());



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

		// TODO スロット
		rhi::SetDescriptorTableParam params[] = {
			{m_dynamicTable, enum_cast(MaterialRootSignatureSlot::DynamicLocal)},
			{m_textureTable, enum_cast(MaterialRootSignatureSlot::TextureLocal)},
			//{m_bufferTable, enum_cast(MaterialRootSignatureSlot::BufferLocal)},
			//{m_samplerTable, enum_cast(MaterialRootSignatureSlot::Sampler)},
		};

		cmdList->setRootDesciptorTable(params, std::size(params));

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
			BindingLayoutDesc desc {
				{
					Binding::ConstantBuffer(1,0),		// グローバルプロパティ(バッファ)
					Binding::Texture(1,0),		// グローバルプロパティ(テクスチャ)
					//RootParameter::Range(DescriptorRangeType::CBV,2,0),		// グローバルプロパティ(テクスチャ)
				},
				{
					Binding::ConstantBuffer(1,1),		// ローカルプロパティ(バッファ)
					Binding::Texture(m_desc.textureProperties.size(),1),		// ローカルプロパティ(テクスチャ)
					//RootParameter::Range(DescriptorRangeType::CBV,m_desc.bufferProperties.size(),1),		// グローバルプロパティ(テクスチャ)
				}
			};
			desc.samplers =
			{
				StaticSamplerDesc(SamplerDesc(TextureFillter::Point),0),	// グローバルプロパティ(サンプラー)
			};
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