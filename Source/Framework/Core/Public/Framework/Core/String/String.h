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

	template<class TChar>
	class StringBase {
	public:
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
			std::conjunction_v<std::is_convertible<const StringViewIsh&, StringViewBase<TChar>>,
			std::negation<std::is_convertible<const StringViewIsh&, const TChar*>>>>;
	public:
		static constexpr size_type npos{ string_type::npos };
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
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
		explicit StringBase(TStringView s) : m_str(s.data(),s.size()){}

		operator StringViewBase<TChar>() const noexcept { return m_str; }
		operator std::basic_string_view<TChar>() const noexcept { return m_str; }


		//===============================================================
		// 演算子
		//===============================================================
		StringBase& operator=(const StringBase& rhs) { m_str = rhs.m_str; return *this; }
		StringBase& operator=(const string_type& rhs) { m_str = rhs; return *this; }
		StringBase& operator=(StringBase&& rhs) noexcept { m_str = rhs.m_str; return *this; }
		StringBase& operator=(string_type&& rhs) noexcept { m_str = m_str; return *this; }
		StringBase& operator=(const value_type* s) { m_str = s; return *this; }
		StringBase& operator=(value_type c) { m_str = c; return *this; }
		StringBase& operator=(std::initializer_list<value_type> il) { m_str = il; return *this; }

		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& operator=(const TStringView& t) { m_str = t; return *this; }


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
		size_type size_bytes() const noexcept { return m_size() * sizeof(value_type); }

		explicit operator bool() const noexcept { return !m_str.empty(); }

		//===============================================================
		// 要素アクセス
		//===============================================================
		value_type& at(size_t pos)& { return m_str.at(pos); }
		const value_type& at(size_t pos) const& { return m_str.at(pos); };
		value_type at(size_t pos)&& { return m_str.at(pos); }
		value_type& operator [](size_t pos)& noexcept { return m_str[pos]; }
		const value_type& operator [](size_t pos) const& noexcept { return m_str[pos]; }
		value_type operator [](size_t pos)&& noexcept { return m_str[pos]; }

		value_type& front() noexcept { return m_str.front(); }
		const value_type& front() const noexcept { return m_front(); }
		value_type& back() noexcept { return m_str.back(); }
		const value_type& back() const noexcept { return m_str.back(); }

		//===============================================================
		// 文字列の変更
		//===============================================================
		StringBase& operator +=(const StringBase& s) { m_str += s; return *this; }
		StringBase& operator +=(const string_type& s) { m_str += s; return *this; }
		StringBase& operator +=(value_type c) { m_str += c; return *this; }
		StringBase& operator +=(const value_type* s) { m_str += s; return *this; }
		StringBase& operator +=(std::initializer_list<value_type> il) { m_str += il; return *this; }
		template <class TStringView>StringBase& operator +=(const TStringView& s) { m_str += s; return *this; }

		StringBase& append(const StringBase& s) { m_str.append(s); return *this; }
		StringBase& append(const string_type& s) { m_str.append(s); return *this; }
		StringBase& append(value_type c) { m_str.append(c); return *this; }
		StringBase& append(const value_type* s) { m_str.append(s); return *this; }
		StringBase& append(const value_type* s, size_t count) { m_str.append(s); return *this; }
		StringBase& append(std::initializer_list<value_type> il) { m_str.append(il); return *this; }
		StringBase& append(size_t n, value_type c) { m_str.append(n, c); return *this; }
		template <class TStringView> StringBase& append(const TStringView& s) { m_str.append(s); return *this; }
		template <class Iterator> StringBase& append(Iterator first, Iterator last) { m_str.append(first, last); return *this; }

		void push_front(value_type c) { m_str.insert(m_str.begin(), c); }
		void push_back(value_type c) { m_str.push_back(c); }

		StringBase& assign(const StringBase& s) { m_str.assign(s.m_str); return *this; }
		StringBase& assign(const string_type& s) { m_str.assign(s); return *this; }
		StringBase& assign(const value_type* s) { m_str.assign(s); return *this; }
		StringBase& assign(size_t count, value_type c) { m_str.assign(count, c); return *this; }
		StringBase& assign(StringBase&& s) noexcept { m_str.assign(s); return *this; }
		StringBase& assign(string_type&& s) noexcept { m_str.assign(s); return *this; }
		StringBase& assign(std::initializer_list<value_type> il) { m_str.assign(il); return *this; }

		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& assign(const TStringView& s) { m_str.assign(s); return *this; }
		template <class Iterator> StringBase& assign(Iterator first, Iterator last) { m_str.assign(first, last); return *this; }

		StringBase& insert(size_t pos, const StringBase& s) { m_str.insert(pos, s); return *this; }
		StringBase& insert(size_t pos, std::initializer_list<value_type> il) { m_str.insert(pos, il); return *this; }
		StringBase& insert(size_t pos, const value_type* s) { m_str.insert(pos, s); return *this; }
		template <class TStringView, class = is_string_view<TStringView>>
		StringBase& insert(size_t pos, const TStringView& s) { m_str.insert(pos, s); return *this; }
		StringBase& insert(size_t pos, size_t n, value_type c) { m_str.insert(pos, n, c); return *this; }
		iterator insert(const_iterator pos, value_type c) { m_str.insert(pos, c); return *this; }
		iterator insert(const_iterator pos, size_t n, value_type c) { m_str.insert(pos, n, c); return *this; }
		template <class Iterator> iterator insert(const_iterator pos, Iterator first, Iterator last) { m_str.insert(pos, first, last); return *this; }
		template <class Iterator> StringBase& insert(const_iterator first1, const_iterator last1, Iterator first2, Iterator last2) { m_str.insert(first1, last1, first2, last2); return *this; }

		StringBase& erase(size_t pos = 0, size_t n = npos) { m_str.erase(pos, n); return *this; }
		iterator erase(const_iterator pos) noexcept { m_str.erase(pos); return *this; }
		iterator erase(const_iterator first, const_iterator last) noexcept { m_str.erase(first, last); return *this; }

		void pop_front() { m_str.pop_front(); }
		void pop_front_n(size_type n) { for (size_type i = 0; i < n; ++i)m_str.push_front(); }
		void pop_back() noexcept { m_str.pop_back(); }
		void pop_back_n(size_type n) noexcept { for (size_type i = 0; i < n; ++i)m_str.push_back(); }

		StringBase& replace(value_type oldChar, value_type newChar) { for (auto& c : m_str)if (c == oldChar)c = newChar; return *this; }

		template <class TStringViewA, class TStringViewB>
		StringBase& replace(const TStringViewA& oldStr, const TStringViewB& newStr) {
			size_type pos = 0;
			while (pos = find(pos, oldStr), pos != m_str.npos) {
				m_str.replace(pos, oldStr.size(), newStr);
				pos += oldStr.size();
			}
			return *this;
		}
		template <class TStringView>
		StringBase& replace(size_type pos, size_type n, const TStringView& s) { m_str.replace(pos, n, s); return *this; }

		template <class TStringView>
		StringBase& replace(const_iterator first, const_iterator last, const TStringView& s) { m_str.replace(first, last, s); return *this; }

		template <class Iterator>
		StringBase& replace(const_iterator first, const_iterator last, Iterator first2, Iterator last2) { m_str.replace(first, last, first2, last2); return *this; }


		// remove
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
		template <class TStringView>
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

		size_type copy(value_type* dst, size_type n, size_type pos = 0) const { return m_view.copy(dst, n, pos); }

		constexpr size_type find(StringViewBase<TChar> s, size_type pos = 0) const noexcept { return m_view.find(s.m_view, pos); }


		constexpr size_type find(value_type ch, size_type pos = 0) const noexcept { return m_view.find(ch, pos); }


		constexpr size_type find(const value_type* s, size_type pos, size_type count) const noexcept { return m_view.find(s, pos, count); }


		constexpr size_type find(const value_type* s, size_type pos = 0) const noexcept { return m_view.find(s, pos); }


		constexpr size_t find_first_not_of(value_type ch, size_t pos = 0) const noexcept { return m_view.find_first_not_of(ch, pos); }


		constexpr size_type rfind(StringViewBase<TChar> s, size_type pos = npos) const noexcept { return m_view.rfind(s.m_view, pos); }


		constexpr size_type rfind(value_type ch, size_type pos = npos) const noexcept { return m_view.rfind(ch, pos); }


		constexpr size_type rfind(const value_type* s, size_type pos, size_type count) const noexcept { return m_view.rfind(s, pos, count); }


		constexpr size_type rfind(const value_type* s, size_type pos = npos) const noexcept { return m_view.rfind(s, pos); }


		constexpr size_t find_last_not_of(value_type ch, size_t pos = npos) const noexcept { return m_view.find_last_not_of(ch, pos); }



		StringViewBase<value_type> substr(size_t pos = 0, size_t n = npos) const { return m_str.substr(pos, n); }
		StringViewBase<value_type> rsubstr(size_t pos = 0, size_t n = npos) const { return m_str.substr(size() - pos - n - 1, n); }
		StringViewBase<value_type> substr_range(size_t first = 0, size_t last = npos) const { return m_str.substr(first, last - first); }

		s32 compare(const StringBase& s) const noexcept { return m_str.compare(s.m_str); }
		s32 compare(StringViewBase<value_type> s) const noexcept { return m_str.compare(s.m_str); }
		s32 compare(const value_type* s) const noexcept { return m_str.compare(s); }

		bool starts_with(value_type c) const noexcept {
			if (empty())return false;
			return m_str.front() == c;
		}
		bool ends_with(value_type c) const noexcept {
			if (empty())return false;
			return m_str.back() == c;
		}
		template <class TStringView> bool starts_with(TStringView s) const {
			if (size() < s.size())return false;
			return s == m_str.substr(0, s.size());
		}
		template <class TStringView> bool ends_with(TStringView s) const {
			if (size() < s.size())return false;
			return s == m_str.substr(size() - s.size(), s.size());
		}
	private:
		string_type m_str;
	};

	//@―---------------------------------------------------------------------------
	//! @brief			具体化
	//@―---------------------------------------------------------------------------
	using String = StringBase<Char>;
	using WString = StringBase<wchar_t>;


}

//! @cond
//===============================================================
// ハッシュ化
//===============================================================
template <>
struct std::hash<ob::core::StringBase<char>>
{
	size_t operator ()(const ob::core::StringViewBase<char>& value) const noexcept
	{
		ob::core::StringViewBase<char> a;

		return std::hash<std::string_view>()({ value.data(), value.size() });
	}
};
template <>
struct std::hash<ob::core::StringBase<wchar_t>>
{
	size_t operator ()(const ob::core::StringBase<wchar_t>& value) const noexcept
	{
		return std::hash<std::wstring_view>()({ value.data(), value.size() });
	}
};
template <>
struct std::hash<ob::core::StringBase<char16_t>>
{
	size_t operator ()(const ob::core::StringBase<char16_t>& value) const noexcept
	{
		return std::hash<std::u16string_view>()({ value.data(), value.size() });
	}
};
template <>
struct std::hash<ob::core::StringBase<char32_t>>
{
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