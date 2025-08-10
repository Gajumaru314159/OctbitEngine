//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/SystemResource.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Sampler.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

	//! @brief  コンストラクタ
	SystemResource::SystemResource(Device& rhi) {
		{
			// テクスチャ
			auto creator = [this,&rhi](IntColor color) {
				Vector<IntColor> colors(32 * 32, color);
				return rhi.createTexture(Format("Color{}",color),TextureType::Texture2D, Size(32, 32), colors);
			};
			m_presetTextures[PresetTexture::White] = creator(IntColor::White);
			m_presetTextures[PresetTexture::Gray] = creator(IntColor::Gray);
			m_presetTextures[PresetTexture::Black] = creator(IntColor::Black);
			m_presetTextures[PresetTexture::Normal] = creator(IntColor::Normal);
		}
		{
			size_t size = 32;
			Vector<IntColor> colors(size * size);
			for (s32 y = 0; y < size; ++y) {
				for (s32 x = 0; x < size; ++x) {
					bool f = (x < size/ 2) ^ (y < size / 2);
					colors[y * size + x] = f ? IntColor::White : IntColor::Gray;
				}
			}
			m_presetTextures[PresetTexture::Check] = rhi.createTexture("Check", TextureType::Texture2D, Size(size, size), colors);
		}

		{
			SamplerDesc desc(TextureFilter::Linear);
			desc.anisotropy = Anisotropy::None;
			desc.filter = TextureFilter::Linear;
			m_presetSamplers[PresetSampler::Default] = rhi.createSampler(desc);
		}
	}

	//! @brief  デストラクタ
	SystemResource::~SystemResource() {
	}

	//! @brief  プリセットテクスチャ取得
	Ref<Texture> SystemResource::getPresetTexture(PresetTexture type) {
		auto found = m_presetTextures.find(type);
		if (found == m_presetTextures.end())
			return nullptr;
		return found->second;
	}

	//! @brief  プリセットサンプラー取得
	Ref<Sampler> SystemResource::getPresetSampler(PresetSampler type) {
		auto found = m_presetSamplers.find(type);
		if (found == m_presetSamplers.end())
			return nullptr;
		return found->second;
	}

}
