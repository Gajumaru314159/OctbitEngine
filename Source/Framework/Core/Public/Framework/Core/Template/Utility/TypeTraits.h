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

    //! @brief イテレータか
	template < class, class = void >
	struct is_iterator : std::false_type {};

	template < typename T >
	struct is_iterator<T,
		std::enable_if_t<
			std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<T>::iterator_category>::value || 
			std::is_base_of<std::output_iterator_tag, typename std::iterator_traits<T>::iterator_category>::value
		>
	> : std::true_type {};


	// メンバ関数ポインタの戻り値の型を取得するためのテンプレート
	template<typename T>
	struct member_function_traits;

	// 特化：メンバ関数ポインタの場合
	template<typename ReturnType, typename ClassType, typename... Args>
	struct member_function_traits<ReturnType(ClassType::*)(Args...)> {
		using return_type = ReturnType;
	};

	// 特化：constメンバ関数ポインタの場合
	template<typename ReturnType, typename ClassType, typename... Args>
	struct member_function_traits<ReturnType(ClassType::*)(Args...) const> {
		using return_type = ReturnType;
	};

}