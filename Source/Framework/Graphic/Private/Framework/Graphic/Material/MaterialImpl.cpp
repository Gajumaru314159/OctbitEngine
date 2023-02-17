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

namespace ob::graphic {

	enum class MaterialRootSignatureSlot {
		Camera,
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
		
		// 最大サイズチェック関数
		auto checkSize = [&bufferSize,&paramSize](const MaterialPropertyDesc& param) {
			bufferSize = std::max<s32>(bufferSize, param.offset+ paramSize);
		};
		auto checkSize2 = [&checkSize,&paramSize](size_t size,const Array<MaterialPropertyDesc> params) {
			paramSize = size;
			std::for_each(params.begin(), params.end(), checkSize);
		};

		checkSize2(sizeof(f32), desc.floatProperties);
		checkSize2(sizeof(Color), desc.colorProperties);
		checkSize2(sizeof(Matrix), desc.matrixProperties);

		//m_textures
		{
			auto bufferDesc = rhi::BufferDesc::Constant(bufferSize,rhi::BindFlag::AllShaderResource);
			m_buffer = rhi::Buffer::Create(bufferDesc);
			OB_ASSERT_EXPR(m_buffer);
			m_bufferBlob.resize(bufferSize);
		}

		{
			m_bufferTable = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
			OB_ASSERT_EXPR(m_bufferTable);
			m_bufferTable->setResource(0, m_buffer);
		}

		{
			m_textureTable = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, desc.textureProperties.size());
			m_samplerTable = rhi::DescriptorTable::Create(DescriptorHeapType::Sampler, desc.textureProperties.size());
			OB_ASSERT_EXPR(m_textureTable);
			OB_ASSERT_EXPR(m_samplerTable);
		}

	}

	void MaterialImpl::creaePipeline() {


		using namespace ob::rhi;

		PipelineStateDesc desc;

		desc.name = Format(TC("Material_Pass"));

		for (auto& [name,pass] : m_desc.passes) {

			//auto [renderPass,subpass] = Material::FindRenderPass(name);
			Ref<RenderPass> renderPass;
			s32 subpass;

			desc.renderPass = renderPass;
			desc.subpass = subpass;

			//desc.rootSignature = ;
			//desc.vertexLayout = TryCreate(m_desc.inputLayout);

			desc.vs = pass.vs;
			desc.ps = pass.ps;

			std::copy(
				std::begin(desc.blend), std::end(desc.blend),
				std::begin(pass.blends)
			);


			desc.rasterizer = pass.rasterizer;
			desc.depthStencil = pass.depthStencil;

			m_passMap[name] = PipelineState::Create(desc);

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
	void MaterialImpl::record(Ref<rhi::CommandList>& cmdList,const Ref<Mesh>& mesh,engine::Name pass) {
		// 1. 定数バッファのデスクリプタ設定
		// 2. テクスチャのデスクリプタ設定
		// 3. サンプラーのデスクリプタ設定
		// 4. バッファのデスクリプタ設定

		// カメラ情報 → RenderSystemやModelで設定する
		// 複数パス → パスを引数に取る
		// シェーダ設定 → 
		// メッシュの描画 → パイプラインごとに頂点レイアウトが違う


		m_buffer->update(m_bufferBlob.size(), m_bufferBlob.data());

		auto found = m_passMap.find(pass);
		if (found == m_passMap.end())
			return;

		// TODO 頂点レイアウト
		auto& pipeline = found->second;
		
		cmdList->setPipelineState(pipeline);
		
		
		// TODO スロット
		rhi::SetDescriptorTableParam params[] = {
			{m_bufferTable, enum_cast(MaterialRootSignatureSlot::Buffer)},
			{m_textureTable, enum_cast(MaterialRootSignatureSlot::Texture)},
			{m_samplerTable, enum_cast(MaterialRootSignatureSlot::Sampler)},
		};
		cmdList->setRootDesciptorTable(params, std::size(params));

	}


}// namespace ob