//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::asset {

	class Asset;

	class AssetFactory {
	public:
		virtual Ref<Asset> create() = 0;
	};


}