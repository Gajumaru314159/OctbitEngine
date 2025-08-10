//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/Texture.h>

namespace ob::rhi {

	//! @brief      プリセットテクスチャ
	enum class PresetTexture {
		White,
		Gray,
		Black,
		Normal,
		Check,
	};

	//! @brief      プリセットサンプラー
	enum class PresetSampler {
		Default
	};

	//! @brief	システムリソース
	class SystemResource : public Singleton<SystemResource> {
	public:

		//! @brief  コンストラクタ
		SystemResource(Device&);

		//! @brief  デストラクタ
		~SystemResource() override;

		//! @brief  プリセットテクスチャ取得
		Ref<Texture> getPresetTexture(PresetTexture type);


		Ref<Sampler> getPresetSampler(PresetSampler type);

	private:

		HashMap<PresetTexture, Ref<Texture>> m_presetTextures;
		HashMap<PresetSampler, Ref<Sampler>> m_presetSamplers;

	};

}
