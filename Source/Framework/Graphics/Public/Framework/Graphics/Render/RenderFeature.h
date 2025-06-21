//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

	//! @brief      描画機能
	//! @details    O3DEでいうところのFeatureProcessor。
	//!				初期状態は非アクティブです。
	//! @ref 		RenderFeature
	class RenderFeature {
	public:

		OB_RTTI();

		virtual ~RenderFeature(){}

		//! @brief      描画	
		virtual void render(FG& fg) {}
		
	};

}