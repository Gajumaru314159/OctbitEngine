//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::asset {

	class Asset;

	class AssetFactory {
	public:
		virtual SPtr<Asset> create() = 0;
	};


}