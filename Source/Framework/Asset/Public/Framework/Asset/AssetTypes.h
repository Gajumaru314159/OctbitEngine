//***********************************************************
//! @file
//! @brief		アセットアイテム
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::asset {

	class Asset;
	OB_EVENT_NOTIFIER(AssetEvent, const SPtr<Asset>&);


}