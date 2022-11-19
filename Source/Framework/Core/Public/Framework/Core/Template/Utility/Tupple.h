﻿//***********************************************************
//! @file
//! @brief		任意型
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <any>

namespace ob::core {

	template<typename... Types>
	using Tuple = std::tuple<Types...>;

}// namespcae ob