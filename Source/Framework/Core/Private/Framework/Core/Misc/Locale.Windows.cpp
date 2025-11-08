//***********************************************************
//! @file
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Misc/Locale.h>
#include <Framework/Core/Template/Utility/Utility.h>

#ifdef OS_WINDOWS
#include <objbase.h>
#endif

#ifdef OS_WINDOWS

namespace ob::core {

	//! @brief システム言語を取得する
	Locale Locale::System() {
		wchar_t localeNameW[LOCALE_NAME_MAX_LENGTH];
		char localeName[LOCALE_NAME_MAX_LENGTH];
		if (GetUserDefaultLocaleName(localeNameW, LOCALE_NAME_MAX_LENGTH)) {
			for (auto i = 0; i < std::size(localeNameW); ++i) {
				localeName[i] = static_cast<char>(localeNameW[i]);
			}
			return Locale(localeName);
		}
		return Locale();
	}

}

#endif