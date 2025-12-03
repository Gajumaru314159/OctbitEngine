//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::asset {

	//! @brief Asset固有ID
	//! @details AssetIdはアセットに対して重複なく割り当てられます。
	//!			 言語別アセットも異なるアセットとしてIDが割り当てられます。
	using AssetId = UUID;

}