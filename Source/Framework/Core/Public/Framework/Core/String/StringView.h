//***********************************************************
//! @file
//! @brief		文字列
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/Format.h>
#include <string_view>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief		文字列ビュー基底
	//@―---------------------------------------------------------------------------
	template<class TChar>
	class StringViewBase {
	public:
		//! @cond 
		using string_view_type = std::basic_string_view<TChar>;
		using traits_type = typename string_view_type::traits_type;
		using value_type = typename string_view_type::value_type;
		using pointer = typename string_view_type::pointer;
		using const_pointer = typename string_view_type::const_pointer;
		using reference = typename string_view_type::reference;
		using const_reference = typename string_view_type::const_reference;
		using const_iterator = typename string_view_type::const_iterator;
		using iterator = typename string_view_type::iterator;
		using const_reverse_iterator = typename string_view_type::const_reverse_iterator;
		using reverse_iterator = typename string_view_type::reverse_iterator;
		using size_type = typename string_view_type::size_type;
		using difference_type = typename string_view_type::difference_type;
	public:
		static constexpr size_type npos{ string_view_type::npos };
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		// 
		// StringBase<TChar>, std::basic_string<TChar>, std::basic_string_view<TChar>,const TChar*
		// などから構築可能です。
		//===============================================================
		constexpr StringViewBase() = default;
		constexpr StringViewBase(const StringViewBase&) = default;
		constexpr StringViewBase(const std::basic_string<TChar>& s) noexcept : m_view(s) {}
		constexpr StringViewBase(const string_view_type& s) noexcept : m_view(s) {}
		constexpr StringViewBase(const value_type* s, size_type length) noexcept :m_view(s, length) {}
		constexpr StringViewBase(const value_type* s) noexcept :m_view(s) {}


		//===============================================================
		// 代入演算子
		// 
		// StringBase<TChar>, std::basic_string<TChar>, std::basic_string_view<TChar>,const TChar*
		// などから代入可能です。
		//===============================================================
		constexpr StringViewBase& operator =(const StringViewBase&) = default;


		//===============================================================
		// 変換
		//===============================================================
		operator std::basic_string_view<TChar>() const noexcept { return m_view; }


		//===============================================================
		// イテレータ
		//===============================================================
		const_iterator begin() const noexcept { return m_view.begin(); }
		const_iterator end() const noexcept { return m_view.end(); }
		const_iterator cbegin() const noexcept { return m_view.cbegin(); }
		const_iterator cend() const noexcept { return m_view.cend(); }
		const_reverse_iterator rbegin() const noexcept { return m_view.rbegin(); }
		const_reverse_iterator rend() const noexcept { return m_view.rend(); }
		const_reverse_iterator crbegin() const noexcept { return m_view.crbegin(); }
		const_reverse_iterator crend() const noexcept { return m_view.crend(); }


		//===============================================================
		// 領域
		//===============================================================
		constexpr size_type size() const noexcept { return m_view.size(); }
		constexpr size_type size_bytes() const noexcept { return m_view.size() * sizeof(TChar); }
		constexpr size_type length() const noexcept { return m_view.length(); }
		constexpr size_type max_size() const noexcept { return m_view.max_size(); }
		constexpr bool empty() const noexcept { return m_view.empty(); }


		//===============================================================
		// 要素アクセス
		//===============================================================
		constexpr const_reference operator [](size_type index) const noexcept { return m_view[index]; }
		constexpr const_reference at(size_type index) const { return m_view.at(index); }

		constexpr const_reference front() const noexcept { return m_view.front(); }
		constexpr const_reference back() const noexcept { return m_view.back(); }

		constexpr const_pointer data() const noexcept { return m_view.data(); }


		//===============================================================
		// 文字列の入れ替え
		//===============================================================
		constexpr void swap(StringViewBase& other) noexcept { m_view.swap(other); }


		//===============================================================
		// 文字列の切り出し
		//===============================================================
		constexpr void remove_prefix(size_type n) noexcept { m_view = m_view.substr(n); }
		constexpr void remove_suffix(size_type n) noexcept { m_view = m_view.substr(0, size() - n); }
		constexpr StringViewBase substr(size_type pos = 0, size_type n = npos) const { return m_view.substr(pos, n); }
		constexpr StringViewBase rsubstr(size_type pos = 0, size_type n = npos) const { return m_str.substr(size() - pos - n - 1, n); }
		constexpr StringViewBase substr_range(size_type first = 0, size_type last = npos) const { return m_str.substr(first, last - first); }
		size_type copy(value_type* dst, size_type n, size_type pos = 0) const { return m_view.copy(dst, n, pos); }

		StringViewBase trim() {
			const auto whiteSpaceDelimiters = TC(" \t\n\r\f\v");
			auto start = m_view.find_first_not_of(whiteSpaceDelimiters);
			auto end = m_view.find_last_not_of(whiteSpaceDelimiters);
			if (start == npos) start = 0;
			return substr(start,end-start+1);
		}

		//===============================================================
		// 文字列の検索
		//===============================================================
		constexpr size_type find(StringViewBase s, size_type pos = 0) const noexcept { return m_view.find(s.m_view, pos); }
		constexpr size_type find(value_type ch, size_type pos = 0) const noexcept { return m_view.find(ch, pos); }
		constexpr size_type find(const value_type* s, size_type pos, size_type count) const noexcept { return m_view.find(s, pos, count); }
		constexpr size_type find(const value_type* s, size_type pos = 0) const noexcept { return m_view.find(s, pos); }
		constexpr size_type find_first_not_of(value_type ch, size_type pos = 0) const noexcept { return m_view.find_first_not_of(ch, pos); }
		constexpr size_type rfind(StringViewBase s, size_type pos = npos) const noexcept { return m_view.rfind(s.m_view, pos); }
		constexpr size_type rfind(value_type ch, size_type pos = npos) const noexcept { return m_view.rfind(ch, pos); }
		constexpr size_type rfind(const value_type* s, size_type pos, size_type count) const noexcept { return m_view.rfind(s, pos, count); }
		constexpr size_type rfind(const value_type* s, size_type pos = npos) const noexcept { return m_view.rfind(s, pos); }
		constexpr size_type find_last_not_of(value_type ch, size_type pos = npos) const noexcept { return m_view.find_last_not_of(ch, pos); }


		//===============================================================
		// 文字列の比較
		//===============================================================
		constexpr s32 compare(StringViewBase s) const noexcept { return m_view.compare(s.m_view); }
		constexpr s32 compare(size_type pos1, size_type n1, StringViewBase s) const noexcept { return m_view.compare(pos1, n1, s.m_view); }
		constexpr s32 compare(size_type pos1, size_type n1, StringViewBase s, size_type pos2, size_type n2) const noexcept { return m_view.compare(pos1, n1, s.m_view, pos2, n2); }
		constexpr s32 compare(const value_type* s) const noexcept { return m_view.compare(s); }
		constexpr s32 compare(size_type pos1, size_type n1, const value_type* s) const noexcept { return m_view.compare(pos1, n1, s); }
		constexpr s32 compare(size_type pos1, size_type n1, const value_type* s, size_type n2) const noexcept { return m_view.compare(pos1, n1, s, n2); }

		constexpr bool starts_with(value_type ch) const noexcept {
			if (empty())return false;
			return m_view.front() == ch;
		}
		constexpr bool starts_with(StringViewBase s) const noexcept {
			if (size() < s.size())return false;
			return m_view.substr(0, s.size()) == s.m_view;
		}

		bool ends_with(value_type ch) const noexcept {
			if (empty())return false;
			return m_view.back() == ch;
		}
		constexpr bool ends_with(StringViewBase s) const noexcept {
			if (size() < s.size())return false;
			return m_view.substr(size() - s.size(), s.size()) == s.m_view;
		}
		constexpr bool contains(StringViewBase s) const noexcept { return m_view.find(s.m_view) != m_view.npos; }
		constexpr bool contains(value_type* c) const noexcept { return m_view.find(c) != m_view.npos; }
		constexpr bool contains(const value_type* s) const noexcept { return m_view.find(s) != m_view.npos; }


		//===============================================================
		// 比較
		//===============================================================
		friend constexpr bool operator ==(StringViewBase lhs, StringViewBase rhs) noexcept { return (lhs.compare(rhs) == 0); }
		friend constexpr bool operator !=(StringViewBase lhs, StringViewBase rhs) noexcept { return (lhs.compare(rhs) != 0); }
		friend constexpr bool operator <(StringViewBase lhs, StringViewBase rhs) noexcept { return (lhs.compare(rhs) < 0); }
		friend constexpr bool operator <=(StringViewBase lhs, StringViewBase rhs) noexcept { return (lhs.compare(rhs) <= 0); }
		friend constexpr bool operator >(StringViewBase lhs, StringViewBase rhs) noexcept { return (lhs.compare(rhs) > 0); }
		friend constexpr bool operator >=(StringViewBase lhs, StringViewBase rhs) noexcept { return (lhs.compare(rhs) >= 0); }

	private:
		string_view_type m_view;
	};

	template<class TChar>
	inline void swap(StringViewBase<TChar>& a, StringViewBase<TChar>& b) noexcept {
		a.swap(b);
	}

	using StringView = StringViewBase<Char>;
	using WStringView = StringViewBase<wchar_t>;

}

//! @cond
//===============================================================
// ハッシュ化
//===============================================================
template <>
struct std::hash<ob::core::StringViewBase<char>>
{
	size_t operator ()(const ob::core::StringViewBase<char>& value) const noexcept
	{
		ob::core::StringViewBase<char> a;

		return std::hash<std::string_view>()({ value.data(), value.size() });
	}
};
template <>
struct std::hash<ob::core::StringViewBase<wchar_t>>
{
	size_t operator ()(const ob::core::StringViewBase<wchar_t>& value) const noexcept
	{
		return std::hash<std::wstring_view>()({ value.data(), value.size() });
	}
};
template <>
struct std::hash<ob::core::StringViewBase<char16_t>>
{
	size_t operator ()(const ob::core::StringViewBase<char16_t>& value) const noexcept
	{
		return std::hash<std::u16string_view>()({ value.data(), value.size() });
	}
};
template <>
struct std::hash<ob::core::StringViewBase<char32_t>>
{
	size_t operator ()(const ob::core::StringViewBase<char32_t>& value) const noexcept
	{
		return std::hash<std::u32string_view>()({ value.data(), value.size() });
	}
};


//===============================================================
// フォーマット
//===============================================================
template <> struct fmt::formatter<ob::core::StringView, ob::core::Char> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const ob::core::StringView& value, FormatContext& ctx) -> decltype(ctx.out()) {
		return format_to(ctx.out(), TC("{}"), std::basic_string_view<ob::core::Char>(value.data(), value.size()));
	}
};
//! @endcond