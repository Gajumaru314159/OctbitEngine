//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/TextureManager.h>
#include <Framework/RHI/RenderTexture.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	TextureManager::TextureManager(Size size)
		: m_size(size)
	{

	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画ターゲットサイズ取得
	//@―---------------------------------------------------------------------------
	Size TextureManager::getSize()const {
		return m_size;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      テクスチャ生成
	//@―---------------------------------------------------------------------------
	Ref<rhi::RenderTexture> TextureManager::createTexture(StringView name, rhi::TextureFormat format, Size size,rhi::ClearColor clearColor) {
		if (auto found = m_textures.find(name); found != m_textures.end()) {

			LOG_ERROR("{}は登録済みのテクスチャです。", name);
			return nullptr;

		}

		rhi::RenderTextureDesc desc;
		desc.name = name;
		desc.format = format;
		desc.size = size;
		desc.clear = clearColor;

		auto texture = rhi::RenderTexture::Create(desc);

		if (texture) {
			m_textures.emplace(name, texture);
		}

		return texture;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      テクスチャ生成
	//@―---------------------------------------------------------------------------
	Ref<rhi::RenderTexture> TextureManager::createTexture(StringView name, rhi::TextureFormat format, Size size) {
		return createTexture(name, format, size, {});
	}

	//@―---------------------------------------------------------------------------
	//! @brief      テクスチャ生成
	//@―---------------------------------------------------------------------------
	Ref<rhi::RenderTexture> TextureManager::createTexture(StringView name, rhi::TextureFormat format) {
		return createTexture(name,format,m_size);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      テクスチャ生成
	//@―---------------------------------------------------------------------------
	Ref<rhi::RenderTexture> TextureManager::createTexture(StringView name, rhi::TextureFormat format, f32 scale) {
		Size size;
		size.width = (s32)Math::Ceil(m_size.width * scale);
		size.height = (s32)Math::Ceil(m_size.height * scale);
		size.depth = (s32)Math::Ceil(m_size.depth * scale);
		return createTexture(name, format, size);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      テクスチャ取得
	//@―---------------------------------------------------------------------------
	Ref<rhi::RenderTexture> TextureManager::findTexture(StringView name) {
		if (auto found = m_textures.find(name); found != m_textures.end()) {
			return found->second;
		}
		return nullptr;
	}

}