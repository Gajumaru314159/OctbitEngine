//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Rendering/Material/MaterialImpl.h>

namespace ob::rendering {

	MaterialImpl::MaterialImpl(const MaterialDesc& desc) {
		using namespace ob::graphic;
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
			auto bufferDesc = graphic::BufferDesc::Constant(bufferSize,graphic::BindFlag::AllShaderResource);
			m_buffer = graphic::Buffer(bufferDesc);
			OB_CHECK_ASSERT_EXPR(m_buffer);
			m_bufferBlob.resize(bufferSize);
		}

		{
			m_bufferTable = graphic::DescriptorTable(DescriptorHeapType::CBV_SRV_UAV, 1);
			OB_CHECK_ASSERT_EXPR(m_bufferTable);
			m_bufferTable.setResource(0, m_buffer);
		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	bool MaterialImpl::hasProprty(StringView name, PropertyType type) {
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
	void MaterialImpl::setVector(StringView name, Vec4 value) {
		setValueProprty(name, PropertyType::Vector, value);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialImpl::setMatrix(StringView name, const Matrix& value) {
		setValueProprty(name, PropertyType::Matrix, value);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialImpl::setTexture(StringView name, const graphic::Texture& value) {
		if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {

			auto& desc = found->second;
			if (desc.type == PropertyType::Texture)return;
			if (!is_in_range(desc.offset, m_textures))return;

			m_textures[desc.offset] = value;

		}
	}

	void MaterialImpl::record(graphic::CommandList&) {
		// 1. 定数バッファのデスクリプタ設定
		// 2. テクスチャのデスクリプタ設定
		// 3. サンプラーのデスクリプタ設定
		// 4. バッファのデスクリプタ設定

		// カメラ情報 → RenderSystemやModelで設定する
		// 複数パス → パスを引数に取る
		// シェーダ設定 → 
		// メッシュの描画 → パイプラインごとに頂点レイアウトが違う
		

		/*
		
		
		m_pipeline->record(recorder);

		recorder->setDescriptor();


		
		*/
	}


}// namespace ob