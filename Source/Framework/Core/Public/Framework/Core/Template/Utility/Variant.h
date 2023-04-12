//***********************************************************
//! @file
//! @brief		バリアント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <variant>

namespace ob::core {

	//! @brief バリアント
	template <class... Types>
	using Variant = std::variant<Types...>;

}