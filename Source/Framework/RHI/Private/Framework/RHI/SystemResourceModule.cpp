//***********************************************************
//! @file
//! @brief		RHI・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/SystemResourceModule.h>
#include <Framework/RHI/Texture.h>
#include <Framework/Engine/ModuleFactory.h>
#include <Framework/Engine/Engine.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	SystemResourceModule::SystemResourceModule()
	{
		GEngine->ref<RHIModule>();
		{
			// テクスチャ
			auto creator = [this](IntColor color) {
				Array<IntColor> colors(32 * 32, color);
				return Texture::Create(Size(32, 32), colors);
			};
			m_presetTextures[PresetTexture::White] = creator(IntColor::White);
			m_presetTextures[PresetTexture::Gray] = creator(IntColor::Gray);
			m_presetTextures[PresetTexture::Black] = creator(IntColor::Black);
			m_presetTextures[PresetTexture::Normal] = creator(IntColor::Normal);
		}
		{
			s32 size = 32;
			Array<IntColor> colors((size_t)size*size);
			for (s32 y = 0; y < size; ++y) {
				for (s32 x = 0; x < size; ++x) {
					bool f = (x % 2) ^ (y % 2);
					colors[(size_t)y * size + x] = f ? IntColor::White : IntColor::Gray;
				}
			}
			m_presetTextures[PresetTexture::Check] = Texture::Create(Size(size, size), colors);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  プリセットテクスチャ取得
	//@―---------------------------------------------------------------------------
	Ref<Texture> SystemResourceModule::getPresetTexture(PresetTexture type) {
		auto found = m_presetTextures.find(type);
		if (found == m_presetTextures.end())
			return nullptr;
		return found->second;
	}

}// namespace ob::rhi

REGISTER_MODULE(ob::rhi::SystemResourceModule);

