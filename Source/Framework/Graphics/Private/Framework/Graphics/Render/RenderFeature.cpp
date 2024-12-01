//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

	//! @brief      描画機能
	//! @details    O3DEでいうところのFeatureProcessor。
	//!				初期状態は非アクティブです。
	class RenderFeature {
	public:

		OB_RTTI();

		virtual ~RenderFeature(){}

		//! @brief      アクティブにする
		virtual void activate() {}

		//! @brief      非アクティブにする
		virtual void deactivate() {}

	};

}