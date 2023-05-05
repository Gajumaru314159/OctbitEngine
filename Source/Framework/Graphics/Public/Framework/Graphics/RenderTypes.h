//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Name.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      Rendererの描画ソート順
	//@―---------------------------------------------------------------------------
	enum class SortingRule {
		None,
		BackToFront,
		FrontToBack,
		Canvas,
	};

}