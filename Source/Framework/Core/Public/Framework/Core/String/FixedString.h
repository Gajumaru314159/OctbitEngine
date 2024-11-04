//***********************************************************
//! @file
//! @brief		固定長文字列
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <string>
#include <ostream>
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/StringView.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief		固定長文字列基底
	//@―---------------------------------------------------------------------------
	template<class TChar, size_t N>
	class FixedStringBase {
		static_assert(0<N);
    public:
        using value_type = TChar;
        using pointer = TChar*;
        using reference = TChar&;
        using difference_type = s64;

		FixedStringBase() {}
		FixedStringBase(StringViewBase<TChar> str) {
			size_t len = std::min(N - 1, str.size());
			memcpy_s(m_data, N, str.data(), len);
			m_data[len + 1] = (TChar)'\0';
		}

		void push_back(TChar c) {
			if (N <= m_size + 1)return;
			m_data[m_size] = c;
			m_size++;
			m_data[m_size] = 0;
		}
		value_type* data() { return m_data; }
		const value_type* data()const { return m_data; }
		size_t size()const { return m_size; }

		value_type& operator[](size_t index) { return m_data[index]; }
		const value_type& operator[](size_t index) const{ return m_data[index]; }
	private:
		TChar m_data[N] = {};
		size_t m_size = 0;
	};

	template<size_t N>
	using FixedString = FixedStringBase<Char,N>;

}
