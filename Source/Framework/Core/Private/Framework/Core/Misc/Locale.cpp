//***********************************************************
//! @file
//! @brief UUID
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Misc/Locale.h>
#include <Framework/Core/Template/Utility/Utility.h>

#ifdef OS_WINDOWS
#include <objbase.h>
#endif

namespace ob::core {

	//! @brief IETFタグからロケールを生成する
	Locale::Locale(StringView tag) {
		if (tag.empty())return;
		m_tag = tag;
		for (size_t i = 0; i < tag.size(); ++i) {
			if (tag[i] == '-') {
				m_subtags.emplace_back<u8>(i);
				if (m_subtags.size() == 5) break;
			}
		}
		m_subtags.emplace_back<u8>(tag.size());
	}

	//! @brief 対象のロケールが自身のロケールに含まれるか
	//! @details @code
	//!          Locale("en").contains(Locale("en-US")) == true
	//!          Locale("en-US").contains(Locale("en")) == false
	//!          Locale("en").contains(Locale("en")) == true
	//!          @endcode
	bool Locale::contains(const Locale& other) const {
		// 長さが短い方がサブタグ数が多い場合は含まれない
		if (other.m_subtags.size() < m_subtags.size()) return false;

		// 先頭が一致しない場合は含まれない
		if (!other.m_tag.starts_with(m_tag)) return false;

		// タグ長が同じ場合は完全一致
		if (other.m_tag.size() == m_tag.size()) return true;

		// タグが続いている場合は含まない
		return other.m_tag[m_tag.size()] == '-';
	}

	//! @brief サブタグを取得する 
	StringView Locale::at(s32 index)const {
		if (!is_in_range(index, m_subtags)) return "";
		auto start = index == 0 ? 0 : m_subtags[index - 1] + 1;
		auto end = m_subtags[index];
		return StringView(m_tag).substr(start, end - start);
	}

	//! @brief システム言語を取得する
	Locale Locale::System() {
#ifdef OS_WINDOWS
		wchar_t localeNameW[LOCALE_NAME_MAX_LENGTH];
		char localeName[LOCALE_NAME_MAX_LENGTH];
		if (GetUserDefaultLocaleName(localeNameW, LOCALE_NAME_MAX_LENGTH)) {
			for (auto i = 0; i < std::size(localeNameW); ++i) {
				localeName[i] = localeNameW[i];
			}
			return Locale(localeName);
		}
#else
		static_assert("未実装");
#endif
		return Locale("");
	}

}