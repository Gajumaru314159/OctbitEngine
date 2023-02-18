//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Material/MaterialImpl.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/Graphic/Mesh/MeshImpl.h>

namespace ob::graphic {

	enum class MaterialRootSignatureSlot {
		Buffer,
		Texture,
		Sampler,
	};



	MaterialImpl::MaterialImpl(const MaterialDesc& desc)
		: m_desc(desc)
	{
		using namespace ob::rhi;
		s32 bufferSize = 0;
		s32 paramSize = 0;
		
		PropertyType type;

		// 最大サイズチェック関数
		//TODO offsetを配列インデックスに変更
		// できればオフセットも隠蔽
		auto checkSize = [this,&type, &bufferSize, &paramSize](const MaterialPropertyDesc& param) {
			bufferSize = std::max<s32>(bufferSize, param.offset + paramSize);
			m_propertyMap.emplace(param.name, ValuePropertyDesc{type,param.offset});
		};
		auto checkSize2 = [&bufferSize, &checkSize, &paramSize](size_t size, const Array<MaterialPropertyDesc> params) {
			if (params.empty())
				return;
			bufferSize = align_up(bufferSize, size);
			paramSize = size;
			std::for_each(params.begin(), params.end(), checkSize);
		};

		type = PropertyType::Float;
		checkSize2(sizeof(f32)*4, desc.floatProperties);
		type = PropertyType::Color;
		checkSize2(sizeof(Color), desc.colorProperties);
		type = PropertyType::Matrix;
		checkSize2(sizeof(Matrix), desc.matrixProperties);

		if (bufferSize) {
			auto bufferDesc = rhi::BufferDesc::Constant(bufferSize, rhi::BindFlag::AllShaderResource);
			m_buffer = rhi::Buffer::Create(bufferDesc);
			OB_ASSERT_EXPR(m_buffer);
			m_bufferBlob.resize(bufferSize);
			memset(m_bufferBlob.data(), 0, m_bufferBlob.size());
		}

		if (bufferSize) {
			m_bufferTable = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
			OB_ASSERT_EXPR(m_bufferTable);
			m_bufferTable->setResource(0, m_buffer);
		}

		if (desc.textureProperties.size()) {
			m_textureTable = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, desc.textureProperties.size());
			m_samplerTable = rhi::DescriptorTable::Create(DescriptorHeapType::Sampler, desc.textureProperties.size());
			OB_ASSERT_EXPR(m_textureTable);
			OB_ASSERT_EXPR(m_samplerTable);
		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	bool MaterialImpl::hasProprty(StringView name, PropertyType type) const {
		if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {
			return found->second.type == type;
		}
		return false;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialImpl::setFloat(StringView name, f32 value) {
		setValueProprty(name, PropertyType::Float, value);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialImpl::setColor(StringView name, Color value) {
		setValueProprty(name, PropertyType::Color, value);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialImpl::setMatrix(StringView name, const Matrix& value) {
		setValueProprty(name, PropertyType::Matrix,
#if 1
			value
#else
			value.transposed()
#endif
		);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialImpl::setTexture(StringView name, const Ref<Texture>& value) {
		if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {

			auto& desc = found->second;
			if (desc.type == PropertyType::Texture)return;
			if (!is_in_range(desc.offset, m_textures))return;

			m_textures[desc.offset] = value;

			m_textureTable->setResource(desc.offset, value);

		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialImpl::record(Ref<rhi::CommandList>& cmdList, const Matrix& matrix, const Ref<Mesh>& mesh, s32 submeshIndex, engine::Name pass) {
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

		auto pipelineItr = m_pipelineMap.find(pMesh->getvertexLayoutId());
		if (pipelineItr == m_pipelineMap.end()) {
			pipeline = createPipeline(pass, pMesh->getVertexLayout(),pMesh->getvertexLayoutId());
		} else {
			pipeline = pipelineItr->second;
		}

		if (!pipeline)
			return;

		cmdList->setPipelineState(pipeline);

		// TODO スロット
		rhi::SetDescriptorTableParam params[] = {
			{m_bufferTable, enum_cast(MaterialRootSignatureSlot::Buffer)},
			{m_textureTable, enum_cast(MaterialRootSignatureSlot::Texture)},
			//{m_samplerTable, enum_cast(MaterialRootSignatureSlot::Sampler)},
		};
		cmdList->setRootDesciptorTable(params, std::size(params));

		pMesh->record(cmdList, submeshIndex);

	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialImpl::record(Ref<rhi::CommandList>& cmdList, Span<Matrix> matrices, const Ref<Mesh>& mesh, s32 submesh, engine::Name pass) {

		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  パイプラインを生成
	//@―---------------------------------------------------------------------------
	Ref<rhi::PipelineState> MaterialImpl::createPipeline(engine::Name pass, const rhi::VertexLayout& layout,VertexLayoutId id) {

		using namespace ob::rhi;

		Ref<rhi::PipelineState> pipeline;

		// マテリアルパス取得
		auto passItr = m_desc.passes.find(pass);

		if (passItr == m_desc.passes.end())
			return nullptr;

		auto& materialPass = passItr->second;

		// レンダーパス取得
		auto [renderPass, subpass] = Material::FindRenderPass(materialPass.renderTag);

		if (!renderPass)
			return nullptr;

		if (!is_in_range(subpass, renderPass->desc().subpasses))
			return nullptr;

		// 頂点レイアウト
		rhi::VertexLayout mapped;

		for (auto& a1 : materialPass.layout.attributes) {

			bool ok = false;
			for (auto& a2 : layout.attributes) {

				if (
					a1.semantic == a2.semantic &&
					a1.type == a2.type &&
					a1.dimention == a2.dimention &&
					a1.index == a2.index
					)
				{
					mapped.attributes.push_back(a2);
					ok = true;
					break;
				}

			}
			if (ok == false) {
				LOG_ERROR("PipelineStateの生成に失敗。マテリアルに必要な頂点情報が足りません。");
				return nullptr;
			}

		}

		// パイプライン
		{
			PipelineStateDesc desc;

			desc.name = TC("Material");
			desc.renderPass = renderPass;
			desc.subpass = subpass;
			//TODO RootSignatureをマテリアル内部に閉じ込める
			desc.rootSignature = materialPass.rootSignature;
			desc.vertexLayout = layout;
			desc.vs = materialPass.vs;
			desc.ps = materialPass.ps;
			desc.blend = materialPass.blends;
			desc.rasterizer = materialPass.rasterizer;
			desc.depthStencil = materialPass.depthStencil;

			pipeline = PipelineState::Create(desc);

			if (pipeline) {
				m_pipelineMap[id] = pipeline;
			}
		}


		return pipeline;

	}


}// namespace ob