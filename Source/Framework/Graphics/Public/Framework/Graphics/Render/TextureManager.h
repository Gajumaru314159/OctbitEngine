//***********************************************************
//! @file
//! @brief		TextureManager
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/RenderTextureDesc.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      TextureManager
	//! @details    1フレーム内でのみ使用するレンダーテクスチャ
	//@―---------------------------------------------------------------------------
	class TextureManager {
	public:

		TextureManager(Size size);

		Size getSize()const;

		//@―---------------------------------------------------------------------------
		//! @brief      テクスチャ生成
		//@―---------------------------------------------------------------------------
		//! @{
		Ref<rhi::RenderTexture> createTexture(StringView name, rhi::TextureFormat format, Size size,rhi::ClearColor clearColor);
		Ref<rhi::RenderTexture> createTexture(StringView name, rhi::TextureFormat format, Size size);
		Ref<rhi::RenderTexture> createTexture(StringView name, rhi::TextureFormat format);
		Ref<rhi::RenderTexture> createTexture(StringView name, rhi::TextureFormat format, f32 scale);
		//! @}

		//@―---------------------------------------------------------------------------
		//! @brief      テクスチャ取得
		//@―---------------------------------------------------------------------------
		Ref<rhi::RenderTexture> findTexture(StringView name);

	private:
		Size m_size;
		Map<String, Ref<rhi::RenderTexture>, std::less<>> m_textures;
	};

}