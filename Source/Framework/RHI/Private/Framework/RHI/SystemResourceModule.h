﻿//***********************************************************
//! @file
//! @brief		RHI・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RHIModule.h>
#include <Framework/RHI/Texture.h>
#include <Framework/Engine/ModuleFactory.h>
#include <Framework/Engine/Engine.h>

namespace ob::rhi {

	class SystemResourceModule :public ob::engine::IModule {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		SystemResourceModule();

		//@―---------------------------------------------------------------------------
		//! @brief  プリセットテクスチャ取得
		//@―---------------------------------------------------------------------------
		Ref<Texture> getPresetTexture(PresetTexture type);

	private:

		HashMap<PresetTexture, Ref<Texture>> m_presetTextures;

	};

}// namespace ob::rhi::dx12
