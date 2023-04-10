//***********************************************************
//! @file
//! @brief		バリアント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <type_traits>

namespace ob::core {

	//! @brief const volatile &を除去
	template <class T>
	using remove_cvr = std::remove_cv<std::remove_reference_t<T>>;

	template <class T>
	using remove_cvr_t = typename remove_cvr<T>::type;

}// namespcae ob