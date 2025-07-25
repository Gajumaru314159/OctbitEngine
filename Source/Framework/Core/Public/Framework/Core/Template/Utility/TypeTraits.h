//***********************************************************
//! @file
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

    //! @brief イテレータか
	template < class, class = void >
	struct is_iterator : std::false_type {};

	//! @cond
	
	template < typename T >
	struct is_iterator<T,
		std::enable_if_t<
			std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category>::value || 
			std::is_base_of<std::output_iterator_tag, typename std::iterator_traits<T>::iterator_category>::value
		>
	> : std::true_type {};

	//! @endcond
}