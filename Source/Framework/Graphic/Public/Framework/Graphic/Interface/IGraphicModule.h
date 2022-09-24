//***********************************************************
//! @file
//! @brief		グラフィック・モジュール
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>
#include <Framework/Graphic/Types/FeatureLevel.h>

namespace ob::graphic {
	class IDevice;
}

namespace ob::graphic {

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
		virtual IDevice* createDevice(FeatureLevel featureLevel = FeatureLevel::Default) = 0;

	protected:
		GraphicModule(engine::Engine& engine)
			:engine::IModule(engine)
		{
		}

	};



}// namespace pb::graphic