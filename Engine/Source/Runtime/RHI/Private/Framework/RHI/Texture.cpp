//***********************************************************
//! @file
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

	//! @brief  空テクスチャを生成
	Ref<Texture> Texture::Create(const TextureDesc& desc) {
		if (auto device = Device::Get()) {
			return device->createTexture(desc);
		}
		return nullptr;
	}

	//! @brief  ビットマップデータを指定して2Dテクスチャを生成
	Ref<Texture> Texture::Create(StringView name, const Bitmap& bitmap) {
		return Create(name, TextureType::Texture2D, bitmap.size(), {bitmap.data(),bitmap.pixelCount()});
	}

	//! @brief 色データから新しいテクスチャを作成
	Ref<Texture> Texture::Create(StringView name, BlobView blob) {
		if (auto device = Device::Get()) {
			return device->createTexture(name, blob);
		}
		return nullptr;
	}

	//! @brief  テクスチャバイナリからテクスチャを生成
	Ref<Texture> Texture::Create(StringView name, TextureType type, Size size, Span<const IntColor> colors) {
		if (auto device = Device::Get()) {
			return device->createTexture(name, type, size, colors);
		}
		return nullptr;
	}

	//! @brief      ファイルパスからテクスチャを読み込む
	Ref<Texture> Texture::Load(StringView path) {

		File fs(path);
		if (fs) {
			Blob blob(fs.size());
			fs.read(blob.data(), blob.size());
			return Texture::Create(path, blob);
		}

		return nullptr;
	}

	//! @brief  ベースのテクスチャを指定して異なるビューを持つテクスチャを作成
	Ref<Texture> Texture::Create(const TextureViewDesc& desc) {
		if (auto device = Device::Get()) {
			return device->createTexture(desc);
		}
		return nullptr;
	}

	bool Texture::Supports(TextureFormat format, TextureType type) {
		if (auto device = Device::Get()) {
			return device->supports(format,type);
		}
		return false;
	}
}