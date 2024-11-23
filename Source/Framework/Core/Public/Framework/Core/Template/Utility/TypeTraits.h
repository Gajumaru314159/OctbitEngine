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

	// 関数のnoexceptを除去する
	template <typename T>
	struct remove_noexcept
	{
		using type = T;
	};
	template <typename TReturn, typename... Args>
	struct remove_noexcept<TReturn(Args...) noexcept>
	{
		using type = TReturn(Args...);
	};
	template <typename TReturn, class OwnerType, typename... Args>
	struct remove_noexcept<TReturn(OwnerType::*)(Args...) noexcept>
	{
		using type = TReturn(OwnerType::*)(Args...);
	};
	template <typename TReturn, class OwnerType, typename... Args>
	struct remove_noexcept<TReturn(OwnerType::*)(Args...) const noexcept>
	{
		using type = TReturn(OwnerType::*)(Args...);
	};
	template <typename T>
	using remove_noexcept_t = typename remove_noexcept<T>::type;


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

	// 特化：メンバ関数ポインタの場合
	template<typename ReturnType, typename ClassType, typename... Args>
	struct member_function_traits<ReturnType(ClassType::*)(Args...)noexcept> {
		using return_type = ReturnType;
	};

	// 特化：constメンバ関数ポインタの場合
	template<typename ReturnType, typename ClassType, typename... Args>
	struct member_function_traits<ReturnType(ClassType::*)(Args...) const noexcept> {
		using return_type = ReturnType;
	};

	// テンプレートメタプログラミングで関数ポインタの引数型を取得
	template<typename T>
	struct arguments_traits;

	// 関数ポインタの特殊化
	template<typename ReturnType, typename... Args>
	struct arguments_traits<ReturnType(*)(Args...)> {
		using argument_types = std::tuple<Args...>;
	};

	// メンバ関数ポインタの特殊化
	template<typename OwnerType, typename ReturnType, typename... Args>
	struct arguments_traits<ReturnType(OwnerType::*)(Args...)> {
		using argument_types = std::tuple<Args...>;
	};

	// constメンバ関数ポインタの特殊化
	template<typename OwnerType, typename ReturnType, typename... Args>
	struct arguments_traits<ReturnType(OwnerType::*)(Args...)const> {
		using argument_types = std::tuple<Args...>;
	};

	// 関数ポインタの特殊化
	template<typename ReturnType, typename... Args>
	struct arguments_traits<ReturnType(*)(Args...)noexcept> {
		using argument_types = std::tuple<Args...>;
	};

	// メンバ関数ポインタの特殊化
	template<typename OwnerType, typename ReturnType, typename... Args>
	struct arguments_traits<ReturnType(OwnerType::*)(Args...)noexcept> {
		using argument_types = std::tuple<Args...>;
	};

	// constメンバ関数ポインタの特殊化
	template<typename OwnerType, typename ReturnType, typename... Args>
	struct arguments_traits<ReturnType(OwnerType::*)(Args...)const noexcept> {
		using argument_types = std::tuple<Args...>;
	};

}