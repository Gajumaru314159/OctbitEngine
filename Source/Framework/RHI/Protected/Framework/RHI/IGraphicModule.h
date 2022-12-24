﻿//***********************************************************
//! @file
//! @brief		グラフィック・モジュール
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>
#include <Framework/RHI/GraphicObjectManager.h>

namespace ob::rhi {
	class IDevice;
}

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief      グラフィック・モジュール
	//! 
	//! @details    グラフィックAPIのモジュール基底です。
	//!             DirectXやVulkanなど、APIを共通化する場合はこのモジュール・インターフェースを実装して下さい。
    //@―---------------------------------------------------------------------------
	class GraphicModule :public engine::IModule {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      デバイス生成
		//@―---------------------------------------------------------------------------
		virtual IDevice* getDevice() = 0;

		//@―---------------------------------------------------------------------------
		//! @brief      
		//@―---------------------------------------------------------------------------
		virtual GraphicObjectManager& getObjectManager() = 0;

	};

}// namespace pb::rhi