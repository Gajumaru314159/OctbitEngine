//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/SystemResource.h>

namespace ob::rhi {

	static Ref<Texture> GetPreset(PresetTexture type) {
		if (auto instance = SystemResource::Get()) {
			return instance->getPresetTexture(type);
		}
		return nullptr;
	}

	Ref<Texture> Texture::White() {
		return GetPreset(PresetTexture::White);
	}
	Ref<Texture> Texture::Gray() {
		return GetPreset(PresetTexture::Gray);
	}
	Ref<Texture> Texture::Black() {
		return GetPreset(PresetTexture::Black);
	}
	Ref<Texture> Texture::Normal() {
		return GetPreset(PresetTexture::Normal);
	}
	Ref<Texture> Texture::Check() {
		return GetPreset(PresetTexture::Check);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//! 
	//! @param desc テクスチャ定義
	//! @param name オブジェクト名
	//@―---------------------------------------------------------------------------
	Ref<Texture> Texture::Create(const TextureDesc& desc) {
		if (auto rhi = RHI::Get()) {
			return rhi->createTexture(desc);
		}
		return nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//! 
	//! @param desc テクスチャ定義
	//! @param name オブジェクト名
	//@―---------------------------------------------------------------------------
	Ref<Texture> Texture::Create(StringView name, BlobView blob) {
		if (auto rhi = RHI::Get()) {
			return rhi->createTexture(name, blob);
		}
		return nullptr;
	}



	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//! 
	//! @param desc テクスチャ定義
	//@―---------------------------------------------------------------------------
	Ref<Texture> Texture::Create(StringView name, Size size, Span<IntColor> colors) {
		if (auto rhi = RHI::Get()) {
			return rhi->createTexture(name, size, colors);
		}
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      パスからテクスチャを読み込み
	//! 
	//! @param path ファイルパス
	//@―---------------------------------------------------------------------------
	Ref<Texture> Texture::Load(const Path& path) {

		FileStream fs(path);
		if (fs) {
			Blob blob(fs.size());
			fs.read(blob.data(), blob.size());
			return Texture::Create(path.string(), blob);
		}

		return nullptr;
	}

}