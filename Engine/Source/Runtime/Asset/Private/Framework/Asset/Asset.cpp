//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Asset/Asset.h>
#include <Framework/Asset/AssetManager.h>

namespace ob::asset {

	void Asset::SetLocale(StringView group, const Locale& locale) {
		if (auto manager = AssetManager::Get()) {
			return manager->setLocale(group,locale);
		}
	}
	Locale Asset::GetLocale(StringView group) {
		if (auto manager = AssetManager::Get()) {
			return std::move(manager->getLocale(group));
		}
		return {};
	}
	Locale Asset::GetAssetLocale(StringView assetType) {
		if (auto manager = AssetManager::Get()) {
			return std::move(manager->getAssetLocale(assetType));
		}
		return {};
	}
}