//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <array>

namespace ob::core {

	//! @brief 静的配列
	template<class T, size_t N>
	using Array = std::array<T, N>;

}