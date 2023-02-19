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

