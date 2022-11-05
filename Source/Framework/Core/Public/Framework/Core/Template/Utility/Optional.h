//***********************************************************
//! @file
//! @brief		オプショナル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <optional>

namespace ob::core {

	//! @brief オプショナル
	template <class T>
	using Optional = std::optional<T>;

}// namespcae ob