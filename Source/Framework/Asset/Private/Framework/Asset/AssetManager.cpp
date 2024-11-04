//***********************************************************
//! @file
//! @brief		アセットアイテム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Asset/AssetManager.h>

namespace {
	static ob::core::Locale s_defaultLocale;
}

namespace ob::asset {

	void AssetManager::setLocale(StringView group, const Locale& locale) {
		String g(group);
		ScopeLock lock(m_localeLock);
		for (auto& asset : m_groupMap[g]) {
			m_localeMapForAssetType[asset] = locale;
		}
		m_localeMapForAssetType[g] = locale;
	}
	Locale AssetManager::getLocale(StringView group) const {
		ScopeLock lock(m_localeLock);
		return try_find(m_localeMapForGroup, group, s_defaultLocale);

	}
	Locale AssetManager::getAssetLocale(StringView group) const {
		ScopeLock lock(m_localeLock);
		return try_find(m_localeMapForAssetType, group, s_defaultLocale);
	}

}