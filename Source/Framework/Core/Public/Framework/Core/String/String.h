//***********************************************************
//! @file
//! @brief		文字列
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <string>
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/StringView.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief		文字列基底
	//! @details	任意の型の文字を文字列として扱えるクラス。
	//!				相互変換可能な文字は char, wchar_t, char16_t, char32_t です。
	//@―---------------------------------------------------------------------------
	template<class TChar>
	class StringBase {
	public:
		//! @cond 
		using string_type = std::basic_string<TChar>;
		using traits_type = typename string_type::traits_type;
		using allocator_type = typename string_type::allocator_type;
		using value_type = typename string_type::value_type;
		using size_type = typename string_type::size_type;
		using difference_type = typename string_type::difference_type;
		using pointer = typename string_type::pointer;
		using const_pointer = typename string_type::const_pointer;
		using reference = typename string_type::reference;
		using const_reference = typename string_type::const_reference;
		using iterator = typename string_type::iterator;
		using const_iterator = typename string_type::const_iterator;
		using reverse_iterator = typename string_type::reverse_iterator;
		using const_reverse_iterator = typename string_type::const_reverse_iterator;

		template <class StringViewIsh>
		using is_string_view = std::enable_if_t<
			std::conjunction_v<
				std::is_convertible<const StringViewIsh&, StringViewBase<TChar>>,
				std::negation<std::is_convertible<const StringViewIsh&, const TChar*>>
			>
		>;
	public:
		static constexpr size_type npos{ string_type::npos };
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		// 
		// StringBase<TChar>, std::basic_string<TChar>, std::basic_string_view<TChar>,const TChar*
		// などから構築可能です。
		//===============================================================
		StringBase() {}
		StringBase(const StringBase& rhs) : m_str(rhs.m_str) {}
		StringBase(const string_type& rhs) : m_str(rhs) {}
		StringBase(const StringBase& rhs, size_type pos) :m_str(rhs.m_str, pos) {}
		StringBase(const string_type& rhs, size_type pos) :m_str(rhs, pos) {}
		StringBase(const StringBase& rhs, size_type pos, size_type n) :m_str(rhs.m_str, pos, n) {}
		StringBase(const string_type& rhs, size_type pos, size_type n) :m_str(rhs, pos, n) {}
		StringBase(const value_type* s) :m_str(s) {}
		StringBase(const value_type* s, size_type n) :m_str(s, n) {}
		StringBase(std::initializer_list<value_type> init) :m_str(init) {}
		StringBase(size_type n, value_type c) :m_str(n, c) {}
		StringBase(StringBase&& rhs) noexcept : m_str(rhs.m_str) {}
		StringBase(string_type&& rhs) noexcept : m_str(rhs) {}

		template <class TStringView, class = is_string_view<TStringView>>
		explicit StringBase(TStringView s) : m_str(std::begin(s), std::end(s)) {}


		//===============================================================
		// 代入演算子
		// 
		// StringBase<TChar>, std::basic_string<TChar>, std::basic_string_view<TChar>,const TChar*
		// などから代入可能です。
		//===============================================================
		StringBase& operator=(const StringBase& rhs) { m_str = rhs.m_str; return *this; }
		StringBase& operator=(const string_type& rhs) { m_str = rhs; return *this; }
		StringBase& operator=(StringBase&& rhs) noexcept { m_str = rhs.m_str; return *this; }
		StringBase& operator=(string_type&& rhs) noexcept { m_str = rhs; return *this; }
		StringBase& operator=(const value_type* s) { m_str = s; return *this; }
		StringBase& operator=(value_type c) { m_str = c; return *this; }
		StringBase& operator=(std::initializer_list<value_type> il) { m_str = il; return *this; }

		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& operator=(const TStringView& t) { m_str = t; return *this; }


		//===============================================================
		// 変換
		//===============================================================
		operator StringViewBase<TChar>() const noexcept { return m_str; }
		operator std::basic_string_view<TChar>() const noexcept { return m_str; }


		//===============================================================
		// イテレータ
		//===============================================================
		iterator begin() noexcept { return m_str.begin(); }
		const_iterator begin() const noexcept { return m_str.begin(); }
		iterator end() noexcept { return m_str.end(); }
		const_iterator end() const noexcept { return m_str.begin(); }
		const_iterator cbegin() const noexcept { return m_str.cbegin(); }
		const_iterator cend() const noexcept { return m_str.cend(); }
		reverse_iterator rbegin() noexcept { return m_str.rbegin(); }
		const_reverse_iterator rbegin() const noexcept { return m_str.rbegin(); }
		reverse_iterator rend() noexcept { return m_str.rend(); }
		const_reverse_iterator rend() const noexcept { return m_str.rend(); }
		const_reverse_iterator crbegin() const noexcept { return m_str.crbegin(); }
		const_reverse_iterator crend() const noexcept { return m_str.crend(); }


		//===============================================================
		// 領域
		//===============================================================
		size_type size() const noexcept { return m_str.size(); }
		size_type length() const noexcept { return m_str.length(); }
		size_type max_size() const noexcept { return m_str.max_size(); }
		void resize(size_type n) { m_str.resize(n); }
		void resize(size_type n, value_type c) { m_str.resize(n, c); }
		size_type capacity() const noexcept { return m_str.capacity(); }
		void reserve(size_type res_arg = 0) { m_str.reserve(res_arg); }
		void shrink_to_fit() { m_str.shrink_to_fit(); }
		void clear() noexcept { m_str.clear(); }
		bool empty() const noexcept { return m_str.empty(); }
		size_type size_bytes() const noexcept { return size() * sizeof(value_type); }


		//===============================================================
		// 要素アクセス
		//===============================================================
		value_type& at(size_type pos)& { return m_str.at(pos); }
		const value_type& at(size_type pos) const& { return m_str.at(pos); };
		value_type at(size_type pos)&& { return m_str.at(pos); }
		value_type& operator [](size_type pos) & noexcept { return m_str[pos]; }
		const value_type& operator [](size_type pos) const& noexcept { return m_str[pos]; }
		value_type operator [](size_type pos) && noexcept { return m_str[pos]; }

		value_type& front() noexcept { return m_str.front(); }
		const value_type& front() const noexcept { return m_front(); }
		value_type& back() noexcept { return m_str.back(); }
		const value_type& back() const noexcept { return m_str.back(); }


		//===============================================================
		// 文字列の追加
		//===============================================================
		StringBase& operator +=(const StringBase& s) { m_str += s; return *this; }
		StringBase& operator +=(const string_type& s) { m_str += s; return *this; }
		StringBase& operator +=(value_type c) { m_str += c; return *this; }
		StringBase& operator +=(const value_type* s) { m_str += s; return *this; }
		StringBase& operator +=(std::initializer_list<value_type> il) { m_str += il; return *this; }

		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& operator +=(const TStringView& s) { m_str += s; return *this; }

		StringBase& append(const StringBase& s) { m_str.append(s); return *this; }
		StringBase& append(const string_type& s) { m_str.append(s); return *this; }
		StringBase& append(value_type c) { m_str.append(c); return *this; }
		StringBase& append(const value_type* s) { m_str.append(s); return *this; }
		StringBase& append(const value_type* s, size_type count) { m_str.append(s); return *this; }
		StringBase& append(std::initializer_list<value_type> il) { m_str.append(il); return *this; }
		StringBase& append(size_type n, value_type c) { m_str.append(n, c); return *this; }
		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& append(const TStringView& s) { m_str.append(s); return *this; }
		template <class Iterator> StringBase& append(Iterator first, Iterator last) { m_str.append(first, last); return *this; }

		void push_front(value_type c) { m_str.insert(m_str.begin(), c); }
		void push_back(value_type c) { m_str.push_back(c); }


		//===============================================================
		// 文字列の設定
		//===============================================================
		StringBase& assign(const StringBase& s) { m_str.assign(s.m_str); return *this; }
		StringBase& assign(const string_type& s) { m_str.assign(s); return *this; }
		StringBase& assign(const value_type* s) { m_str.assign(s); return *this; }
		StringBase& assign(size_type count, value_type c) { m_str.assign(count, c); return *this; }
		StringBase& assign(StringBase&& s) noexcept { m_str.assign(s); return *this; }
		StringBase& assign(string_type&& s) noexcept { m_str.assign(s); return *this; }
		StringBase& assign(std::initializer_list<value_type> il) { m_str.assign(il); return *this; }

		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& assign(const TStringView& s) { m_str.assign(s); return *this; }
		template <class Iterator> StringBase& assign(Iterator first, Iterator last) { m_str.assign(first, last); return *this; }


		//===============================================================
		// 文字列の挿入
		//===============================================================
		StringBase& insert(size_type pos, const StringBase& s) { m_str.insert(pos, s); return *this; }
		StringBase& insert(size_type pos, std::initializer_list<value_type> il) { m_str.insert(pos, il); return *this; }
		StringBase& insert(size_type pos, const value_type* s) { m_str.insert(pos, s); return *this; }
		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& insert(size_type pos, const TStringView& s) { m_str.insert(pos, s); return *this; }
		StringBase& insert(size_type pos, size_type n, value_type c) { m_str.insert(pos, n, c); return *this; }
		iterator insert(const_iterator pos, value_type c) { m_str.insert(pos, c); return *this; }
		iterator insert(const_iterator pos, size_type n, value_type c) { m_str.insert(pos, n, c); return *this; }
		template <class Iterator> iterator insert(const_iterator pos, Iterator first, Iterator last) { m_str.insert(pos, first, last); return *this; }
		template <class Iterator> StringBase& insert(const_iterator first1, const_iterator last1, Iterator first2, Iterator last2) { m_str.insert(first1, last1, first2, last2); return *this; }


		//===============================================================
		// 文字列の消去
		//===============================================================
		StringBase& erase(size_type pos = 0, size_type n = npos) { m_str.erase(pos, n); return *this; }
		iterator erase(const_iterator pos) noexcept { m_str.erase(pos); return *this; }
		iterator erase(const_iterator first, const_iterator last) noexcept { m_str.erase(first, last); return *this; }

		void pop_front() { m_str.pop_front(); }
		void pop_front_n(size_type n) { for (size_type i = 0; i < n; ++i)m_str.push_front(); }
		void pop_back() noexcept { m_str.pop_back(); }
		void pop_back_n(size_type n) noexcept { for (size_type i = 0; i < n; ++i)m_str.pop_back(); }

		StringBase& remove(value_type c) {
			size_type i, n;
			for (n = i = 0; i + n < m_str.size();) {
				m_str[i] = m_str[i + n];
				if (m_str[i] == c)
					n++;
				else
					i++;
			}
			m_str.resize(i);
			return *this;
		}
		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& remove(const TStringView& s) {
			size_type i, n;
			for (n = i = 0; i + n < m_str.size();) {

				m_str[i] = m_str[i + n];

				if (substr(i).starts_with(s)) {
					n += s.size();
				} else {
					i++;
				}

			}
			m_str.resize(i);
			return *this;
		}


		//===============================================================
		// 文字列の置換
		//===============================================================
		StringBase& replace(value_type oldChar, value_type newChar) { for (auto& c : m_str)if (c == oldChar)c = newChar; return *this; }
		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& replace(const TStringView& oldStr, const TStringView& newStr) {
			size_type pos = 0;
			while (pos = find(pos, oldStr), pos != m_str.npos) {
				m_str.replace(pos, oldStr.size(), newStr);
				pos += oldStr.size();
			}
			return *this;
		}
		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& replace(size_type pos, size_type n, const TStringView& s) { m_str.replace(pos, n, s); return *this; }
		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& replace(const_iterator first, const_iterator last, const TStringView& s) { m_str.replace(first, last, s); return *this; }
		template <class Iterator>
		StringBase& replace(const_iterator first, const_iterator last, Iterator first2, Iterator last2) { m_str.replace(first, last, first2, last2); return *this; }


		//===============================================================
		// 文字列の入れ替え
		//===============================================================
		void swap(StringBase& other) noexcept { std::swap(m_str, other.m_str); }
		void swap(string_type& other) noexcept { std::swap(m_str, other); }


		//===============================================================
		// 文字列の操作
		//===============================================================
		const value_type* c_str() const noexcept { return m_str.c_str(); }
		const value_type* data() const noexcept { return m_str.data(); }
		value_type* data() noexcept { return m_str.data(); }

		string_type& str() noexcept { return m_str; }
		const string_type& str() const noexcept { return m_str; }

		allocator_type get_allocator() const noexcept { return n_str.get_allocator(); }

		size_type copy(value_type* dst, size_type n, size_type pos = 0) const { return m_str.copy(dst, n, pos); }


		//===============================================================
		// 文字列の切り出し
		//===============================================================
		StringViewBase<TChar> substr(size_type pos = 0, size_type n = npos) const { return StringViewBase<TChar>(m_str).substr(pos, n); }
		StringViewBase<TChar> rsubstr(size_type pos = 0, size_type n = npos) const { return StringViewBase<TChar>(m_str).substr(size() - pos - n - 1, n); }
		StringViewBase<TChar> substr_range(size_type first = 0, size_type last = npos) const { return StringViewBase<TChar>(m_str).substr(first, last - first); }


		//===============================================================
		// 文字列の検索
		//===============================================================
		template <class TStringView, class = is_string_view<TStringView>>
		constexpr size_type find(TStringView s, size_type pos = 0) const noexcept { return m_str.find(s.m_str, pos); }
		constexpr size_type find(value_type ch, size_type pos = 0) const noexcept { return m_str.find(ch, pos); }
		constexpr size_type find(const value_type* s, size_type pos, size_type count) const noexcept { return m_str.find(s, pos, count); }
		constexpr size_type find(const value_type* s, size_type pos = 0) const noexcept { return m_str.find(s, pos); }
		constexpr size_type find_first_not_of(value_type ch, size_type pos = 0) const noexcept { return m_str.find_first_not_of(ch, pos); }

		template <class TStringView, class = is_string_view<TStringView>>
		constexpr size_type rfind(TStringView s, size_type pos = npos) const noexcept { return m_str.rfind(s.m_str, pos); }
		constexpr size_type rfind(value_type ch, size_type pos = npos) const noexcept { return m_str.rfind(ch, pos); }
		constexpr size_type rfind(const value_type* s, size_type pos, size_type count) const noexcept { return m_str.rfind(s, pos, count); }
		constexpr size_type rfind(const value_type* s, size_type pos = npos) const noexcept { return m_str.rfind(s, pos); }
		constexpr size_type find_last_not_of(value_type ch, size_type pos = npos) const noexcept { return m_str.find_last_not_of(ch, pos); }


		//===============================================================
		// 文字列の比較
		//===============================================================
		s32 compare(const StringBase& s) const noexcept { return m_str.compare(s.m_str); }
		s32 compare(const value_type* s) const noexcept { return m_str.compare(s); }
		template <class TStringView, class = is_string_view<TStringView>>
		s32 compare(TStringView s) const noexcept { return m_str.compare(s.m_str); }

		bool starts_with(value_type c) const noexcept {
			if (empty())return false;
			return m_str.front() == c;
		}
		bool ends_with(value_type c) const noexcept {
			if (empty())return false;
			return m_str.back() == c;
		}
		template <class TStringView, class = is_string_view<TStringView>>
		bool starts_with(TStringView s) const {
			if (size() < s.size())return false;
			return s == m_str.substr(0, s.size());
		}

		template <class TStringView, class = is_string_view<TStringView>>
		bool ends_with(TStringView s) const {
			if (size() < s.size())return false;
			return s == m_str.substr(size() - s.size(), s.size());
		}

		template <class TStringView, class = is_string_view<TStringView>>
		constexpr bool contains(TStringView s) const noexcept { return m_str.find(s) != m_str.npos; }
		constexpr bool contains(value_type* c) const noexcept { return m_str.find(c) != m_str.npos; }
		constexpr bool contains(const value_type* s) const noexcept { return m_str.find(s) != m_str.npos; }


		//===============================================================
		// 比較
		//===============================================================
		friend inline bool operator ==(const StringBase& lhs, const value_type* rhs) { return lhs.m_str.compare(rhs) == 0; }
		friend inline bool operator ==(const StringBase& lhs, const StringBase& rhs) noexcept { return lhs.m_str.compare(rhs.m_str) == 0; }
		friend inline bool operator ==(const value_type* lhs, const StringBase& rhs) { return rhs == lhs; }
		friend inline bool operator !=(const StringBase& lhs, const StringBase& rhs) noexcept { return lhs.m_str.compare(rhs.m_str) != 0; }
		friend inline bool operator !=(const StringBase& lhs, const value_type* rhs) { return lhs.m_str.compare(rhs) != 0; }
		friend inline bool operator !=(const value_type* lhs, const StringBase& rhs) { return rhs != lhs; }
		friend inline bool operator <(const StringBase& lhs, const StringBase& rhs) noexcept { return lhs.m_str.compare(rhs.m_str) < 0; }
		friend inline bool operator <(const StringBase& lhs, const value_type* rhs) { return lhs.m_str.compare(rhs) < 0; }
		friend inline bool operator <(const value_type* lhs, const StringBase& rhs) { return !(rhs <= lhs); }
		friend inline bool operator >(const StringBase& lhs, const StringBase& rhs) noexcept { return lhs.m_str.compare(rhs.m_str) > 0; }
		friend inline bool operator >(const StringBase& lhs, const value_type* rhs) { return lhs.m_str.compare(rhs) > 0; }
		friend inline bool operator >(const value_type* lhs, const StringBase& rhs) { return !(rhs <= lhs); }
		friend inline bool operator <=(const StringBase& lhs, const StringBase& rhs) noexcept { return lhs.m_str.compare(rhs.m_str) <= 0; }
		friend inline bool operator <=(const StringBase& lhs, const value_type* rhs) { return lhs.m_str.compare(rhs) <= 0; }
		friend inline bool operator <=(const value_type* lhs, const StringBase& rhs) { return !(rhs > lhs); }
		friend inline bool operator >=(const StringBase& lhs, const StringBase& rhs) noexcept { return lhs.m_str.compare(rhs.m_str) >= 0; }
		friend inline bool operator >=(const StringBase& lhs, const value_type* rhs) { return lhs.m_str.compare(rhs) >= 0; }
		friend inline bool operator >=(const value_type* lhs, const StringBase& rhs) { return !(rhs < lhs); }
		friend inline StringBase operator +(const value_type lhs, const StringBase& rhs) { return StringBase(rhs).push_front(lhs); }
		friend inline StringBase operator +(const value_type lhs, StringBase&& rhs) { return StringBase(rhs).push_front(lhs); }
		friend inline StringBase operator +(const value_type* lhs, const StringBase& rhs) { return StringBase(lhs) + StringBase(rhs); }
		friend inline StringBase operator +(const value_type* lhs, StringBase&& rhs) { return StringBase(lhs) + StringBase(rhs); }
		friend inline StringBase operator +(StringView lhs, const StringBase& rhs) { return StringBase(lhs) + StringBase(rhs); }
		friend inline StringBase operator +(StringView lhs, StringBase&& rhs) { return StringBase(lhs) + StringBase(rhs); }
		friend inline StringBase operator +(const StringBase& lhs, const value_type rhs) { return StringBase(lhs) + StringBase(rhs); }
		friend inline StringBase operator +(const StringBase& lhs, const value_type* rhs) { return StringBase(lhs) + StringBase(rhs); }
		friend inline StringBase operator +(const StringBase& lhs, StringView rhs) { return StringBase(lhs) + StringBase(rhs); }
		friend inline StringBase operator +(const StringBase& lhs, const StringBase& rhs) { return StringBase(lhs) + StringBase(rhs); }
		friend inline StringBase operator +(const StringBase& lhs, StringBase&& rhs) { return StringBase(lhs) + StringBase(rhs); }
		friend inline StringBase operator +(StringBase&& lhs, const value_type rhs) { return StringBase(lhs).append(rhs); }
		friend inline StringBase operator +(StringBase&& lhs, const value_type* rhs) { return StringBase(lhs).append(rhs); }
		friend inline StringBase operator +(StringBase&& lhs, StringView rhs) { return StringBase(lhs).append(rhs); }
		friend inline StringBase operator +(StringBase&& lhs, const StringBase& rhs) { return StringBase(lhs).append(rhs); }
		friend inline StringBase operator +(StringBase&& lhs, StringBase&& rhs) { return StringBase(lhs).append(rhs); }

	private:
		string_type m_str;
		//! @endcond
	};

	//@―---------------------------------------------------------------------------
	//! @brief			文字列
	//@―---------------------------------------------------------------------------
	using String = StringBase<Char>;

	//@―---------------------------------------------------------------------------
	//! @brief			ネイティブ文字列
	//@―---------------------------------------------------------------------------
	using NativeString = StringBase<NativeChar>;

	//@―---------------------------------------------------------------------------
	//! @brief			ワイド文字列
	//@―---------------------------------------------------------------------------
	using WString = StringBase<wchar_t>;

	//@―---------------------------------------------------------------------------
	//! @brief			UTF-8文字列
	//@―---------------------------------------------------------------------------
	using U8String = StringBase<char>;

	//@―---------------------------------------------------------------------------
	//! @brief			UTF-16文字列
	//@―---------------------------------------------------------------------------
	using U16String = StringBase<char16_t>;

	//@―---------------------------------------------------------------------------
	//! @brief			UTF-16文字列
	//@―---------------------------------------------------------------------------
	using U32String = StringBase<char32_t>;

}

//! @cond
//===============================================================
// ハッシュ化
//===============================================================
template <>
struct std::hash<ob::core::StringBase<char>> {
	size_t operator ()(const ob::core::StringViewBase<char>& value) const noexcept
	{
		ob::core::StringViewBase<char> a;

		return std::hash<std::string_view>()({ value.data(), value.size() });
	}
};
template <>
struct std::hash<ob::core::StringBase<wchar_t>> {
	size_t operator ()(const ob::core::StringBase<wchar_t>& value) const noexcept
	{
		return std::hash<std::wstring_view>()({ value.data(), value.size() });
	}
};
template <>
struct std::hash<ob::core::StringBase<char16_t>> {
	size_t operator ()(const ob::core::StringBase<char16_t>& value) const noexcept
	{
		return std::hash<std::u16string_view>()({ value.data(), value.size() });
	}
};
template <>
struct std::hash<ob::core::StringBase<char32_t>> {
	size_t operator ()(const ob::core::StringBase<char32_t>& value) const noexcept
	{
		return std::hash<std::u32string_view>()({ value.data(), value.size() });
	}
};


//===============================================================
// フォーマット
//===============================================================
template <> struct fmt::formatter<ob::core::String, ob::core::Char> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const ob::core::String& value, FormatContext& ctx) -> decltype(ctx.out()) {
		return format_to(ctx.out(), TC("{}"), ob::core::StringView(value.data(), value.size()));
	}
};
//! @endcond