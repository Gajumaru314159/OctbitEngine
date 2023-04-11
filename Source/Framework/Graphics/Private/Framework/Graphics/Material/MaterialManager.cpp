//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/Graphics/GraphicModule.h>
#include <Framework/Engine/Engine.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/CommandList.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//!	@brief			取得
	//@―---------------------------------------------------------------------------
	MaterialManager& MaterialManager::Get() {
		static auto module = GEngine->get2<GraphicModule>();
		OB_ASSERT_EXPR(module);
		return module->getMaterialManager();
	}


	//@―---------------------------------------------------------------------------
	//!	@brief			コンストラクタ
	//@―---------------------------------------------------------------------------
	MaterialManager::MaterialManager() {
		// グローバル変数用のリソース生成
		constexpr size_t GLOBAL_TEXTURE_MAX = 256;
		constexpr size_t GLOBAL_FLOAT_MAX = 256;
		constexpr size_t GLOBAL_COLOR_MAX = 32;
		constexpr size_t GLOBAL_MATRIX_MAX = 32;

		m_textures.resize(GLOBAL_TEXTURE_MAX);

		using namespace ob::rhi;

		constexpr size_t BUFFER_SIZE = sizeof(f32) * GLOBAL_FLOAT_MAX + sizeof(Color) * GLOBAL_COLOR_MAX + sizeof(Matrix) * GLOBAL_MATRIX_MAX;

		{
			auto bufferDesc = rhi::BufferDesc::Constant(BUFFER_SIZE, rhi::BindFlag::AllShaderResource);
			m_buffer = rhi::Buffer::Create(bufferDesc);
			OB_ASSERT_EXPR(m_buffer);
			m_bufferBlob.resize(BUFFER_SIZE);
			memset(m_bufferBlob.data(), 0, m_bufferBlob.size());
		}

		{
			m_bufferTable = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
			OB_ASSERT_EXPR(m_bufferTable);
			m_bufferTable->setResource(0, m_buffer);
		}

		{
			m_textureTable = rhi::DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, GLOBAL_TEXTURE_MAX);

			// サンプラのパターン数は限られる
			///m_samplerTable = rhi::DescriptorTable::Create(DescriptorHeapType::Sampler, GLOBAL_TEXTURE_MAX);
			OB_ASSERT_EXPR(m_textureTable);
			//OB_ASSERT_EXPR(m_samplerTable);

			for (s32 i = 0; i < GLOBAL_TEXTURE_MAX; ++i) {
				m_textureTable->setResource(i, rhi::Texture::White());
			}
		}
		m_propertyMap.emplace(TC("LightDir"), ValuePropertyDesc{ PropertyType::Color,0 });
		m_propertyMap.emplace(TC("Matrix"), ValuePropertyDesc{ PropertyType::Matrix,sizeof(Color)});
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			デストラクタ
	//@―---------------------------------------------------------------------------
	MaterialManager::~MaterialManager() {
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			描画タグにRenderPassを登録
	//@―---------------------------------------------------------------------------
	void MaterialManager::registerRenderPass(engine::Name name, const Ref<rhi::RenderPass>& renderPass, s32 subpass) {
		m_renderPassMap.emplace(name, rhi::SubPass{ renderPass, subpass });
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			描画タグからRenderPassを登録
	//@―---------------------------------------------------------------------------
	rhi::SubPass MaterialManager::FindRenderPass(engine::Name renderTag) {
		auto found = m_renderPassMap.find(renderTag);
		if (found == m_renderPassMap.end())
			return {nullptr,0};
		return found->second;
	}


	//@―---------------------------------------------------------------------------
	//!	@brief			レイアウトID取得
	//@―---------------------------------------------------------------------------
	VertexLayoutId MaterialManager::getVertexLayoutId(const rhi::VertexLayout& layout) {
		ScopeLock lock(m_lock);
		auto found = m_map.find(layout);
		if (found == m_map.end()) {
			return m_map[layout] = static_cast<VertexLayoutId>(m_map.size());
		} else {
			return found->second;
		}
	}




	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	bool MaterialManager::hasProprty(StringView name, PropertyType type) const {
		if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {
			return found->second.type == type;
		}
		return false;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialManager::setFloat(StringView name, f32 value) {
		setValueProprty(name, PropertyType::Float, value);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialManager::setColor(StringView name, Color value) {
		setValueProprty(name, PropertyType::Color, value);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialManager::setMatrix(StringView name, const Matrix& value) {
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
	void MaterialManager::setTexture(StringView name, const Ref<rhi::Texture>& value) {
		if (auto found = m_propertyMap.find(name); found != m_propertyMap.end()) {

			auto& desc = found->second;
			if (desc.type != PropertyType::Texture)return;
			if (!is_in_range(desc.offset, m_textures))return;

			m_textures[desc.offset] = value;

			m_textureTable->setResource(desc.offset, value);

		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  
	//@―---------------------------------------------------------------------------
	void MaterialManager::recordGlobalShaderProperties(Ref<rhi::CommandList>& cmdList) {

		if (!cmdList)return;

		m_buffer->update(m_bufferBlob.size(), m_bufferBlob.data());

		rhi::SetDescriptorTableParam params[]{
			{m_bufferTable,enum_cast(MaterialRootSignatureSlot::BufferGlobal)},
			{m_textureTable,enum_cast(MaterialRootSignatureSlot::TextureGlobal)},
			//{m_textureTable,1},
			//{m_samplerTable,2},
		};
		cmdList->setRootDesciptorTable(params,std::size(params));

	}


}// namespace ob