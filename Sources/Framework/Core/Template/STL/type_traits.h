//***********************************************************
//! @file
//! @brief		型の特性
//! @author		Gajumaru
//***********************************************************
#pragma once
//! @cond
#include <type_traits>

namespace ob::core {

    //===============================================================
    // ヘルパークラス
    //===============================================================
    using std::integral_constant;
    using std::bool_constant;
    using std::true_type;
    using std::false_type;


    //===============================================================
    // 基本的な型
    //===============================================================
    using std::is_void;
    template <class T>
    inline constexpr bool is_void_v = is_void<T>::value;

    using std::is_null_pointer;
    template <class T>
    inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

    using std::is_integral;
    template <class T>
    inline constexpr bool is_integral_v = is_integral<T>::value;

    using std::is_floating_point;
    template <class T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

    using std::is_array;
    template <class T>
    inline constexpr bool is_array_v = is_array<T>::value;

    using std::is_pointer;
    template <class T>
    inline constexpr bool is_pointer_v = is_pointer<T>::value;

    using std::is_lvalue_reference;
    template <class T>
    inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

    using std::is_rvalue_reference;
    template <class T>
    inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

    using std::is_member_object_pointer;
    template <class T>
    inline constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;

    using std::is_member_function_pointer;
    template <class T>
    inline constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;

    using std::is_enum;
    template <class T>
    inline constexpr bool is_enum_v = is_enum<T>::value;

    using std::is_union;
    template <class T>
    inline constexpr bool is_union_v = is_union<T>::value;

    using std::is_class;
    template <class T>
    inline constexpr bool is_class_v = is_class<T>::value;

    using std::is_function;
    template <class T>
    inline constexpr bool is_function_v = is_function<T>::value;


    //===============================================================
    // 組み合わせた型
    //===============================================================
    using std::is_reference;
    template <class T>
    inline constexpr bool is_reference_v = is_reference<T>::value;

    using std::is_arithmetic;
    template <class T>
    inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

    using std::is_fundamental;
    template <class T>
    inline constexpr bool is_fundamental_v = is_fundamental<T>::value;

    using std::is_object;
    template <class T>
    inline constexpr bool is_object_v = is_object<T>::value;

    using std::is_scalar;
    template <class T>
    inline constexpr bool is_scalar_v = is_scalar<T>::value;

    using std::is_compound;
    template <class T>
    inline constexpr bool is_compound_v = is_compound<T>::value;

    using std::is_member_pointer;
    template <class T>
    inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

    //===============================================================
    // 型の特性
    //===============================================================
    using std::is_const;
    template <class T>
    inline constexpr bool is_const_v = is_const<T>::value;

    using std::is_volatile;
    template <class T>
    inline constexpr bool is_volatile_v = is_volatile<T>::value;

    using std::is_trivial;
    template <class T>
    inline constexpr bool is_trivial_v = is_trivial<T>::value;

    using std::is_trivially_copyable;
    template <class T>
    inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

    using std::is_standard_layout;
    template <class T>
    inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

    using std::is_empty;
    template <class T>
    inline constexpr bool is_empty_v = is_empty<T>::value;

    using std::is_polymorphic;
    template <class T>
    inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

    using std::is_abstract;
    template <class T>
    inline constexpr bool is_abstract_v = is_abstract<T>::value;

    using std::is_final;
    template <class T>
    inline constexpr bool is_final_v = is_final<T>::value;

    using std::is_aggregate;
    template <class T>
    inline constexpr bool is_aggregate_v = is_aggregate<T>::value;

    using std::is_signed;
    template <class T>
    inline constexpr bool is_signed_v = is_signed<T>::value;

    using std::is_unsigned;
    template <class T>
    inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

    using std::is_constructible;
    template <class T, class... Args>
    inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

    using std::is_default_constructible;
    template <class T>
    inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;

    using std::is_copy_assignable;
    template <class T>
    inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

    using std::is_move_assignable;
    template <class T>
    inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

    using std::is_destructible;
    template <class T, class U>
    inline constexpr bool is_assignable_v = is_assignable<T, U>::value;

    using std::is_trivially_constructible;
    template <class T>
    inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

    using std::is_trivially_default_constructible;
    template <class T>
    inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

    using std::is_trivially_copy_constructible;
    template <class T>
    inline constexpr bool is_destructible_v = is_destructible<T>::value;

    using std::is_trivially_move_constructible;
    template <class T, class... Args>
    inline constexpr bool is_trivially_constructible_v = is_trivially_constructible<T, Args...>::value;

    using std::is_trivially_assignable;
    template <class T>
    inline constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;

    using std::is_trivially_copy_assignable;
    template <class T>
    inline constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;

    using std::is_trivially_move_assignable;
    template <class T>
    inline constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;

    using std::is_trivially_destructible;
    template <class T>
    inline constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

    using std::is_trivially_assignable;
    template <class T, class U>
    inline constexpr bool is_trivially_assignable_v = is_trivially_assignable<T, U>::value;

    using std::is_nothrow_constructible;
    template <class T, class... Args>
    inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;

    using std::is_nothrow_default_constructible;
    template <class T>
    inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

    using std::is_nothrow_copy_constructible;
    template <class T>
    inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

    using std::is_nothrow_move_constructible;
    template <class T>
    inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;

    using std::is_nothrow_assignable;
    template <class T, class U>
    inline constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;

    using std::is_nothrow_copy_assignable;
    template <class T>
    inline constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;

    using std::is_nothrow_move_assignable;
    template <class T>
    inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

    using std::is_nothrow_destructible;
    template <class T>
    inline constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

    using std::has_virtual_destructor;
    template <class T>
    inline constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;

    using std::is_swappable_with;
    template <class T, class U>
    inline constexpr bool is_swappable_with_v = std::is_swappable_with<T, U>::value;

    using std::is_swappable;
    template <class T>
    inline constexpr bool is_swappable_v = std::is_swappable<T>::value;

    using std::is_nothrow_swappable_with;
    template <class T, class U>
    inline constexpr bool is_nothrow_swappable_with_v = std::is_nothrow_swappable_with<T, U>::value;

    using std::is_nothrow_swappable;
    template <class T, class U>
    inline constexpr bool is_nothrow_swappable_v = std::is_nothrow_swappable<T, U>::value;

    using std::has_unique_object_representations;
    template <class T>
    inline constexpr bool has_unique_object_representations_v = std::has_unique_object_representations<T>::value;


    //===============================================================
    // 型の特性についての問い合わせ
    //===============================================================
    using std::alignment_of;
    template <class T>
    inline constexpr std::size_t alignment_of_v = alignment_of<T>::value;

    using std::rank;
    template <class T>
    inline constexpr std::size_t rank_v = rank<T>::value;

    using std::extent;
    template <class T, unsigned I = 0>
    constexpr std::size_t extent_v = extent<T, I>::value;


    //===============================================================
    // 型の関係
    //===============================================================
    using std::is_same;
    template <class T, class U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

    using std::is_base_of;
    template <class Base, class Derived>
    inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

    using std::is_convertible;
    template <class From, class To>
    inline constexpr bool is_convertible_v = is_convertible<From, To>::value;


    //===============================================================
    // const-volatileの変更
    //===============================================================
    using std::remove_const;
    template <class T>
    using remove_const_t = typename remove_const<T>::type;

    using std::remove_volatile;
    template <class T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    using std::remove_cv;
    template <class T>
    using remove_cv_t = typename remove_cv<T>::type;

    using std::add_const;
    template <class T>
    using add_const_t = typename add_const<T>::type;

    using std::add_volatile;
    template <class T>
    using add_volatile_t = typename add_volatile<T>::type;

    using std::add_cv;
    template <class T>
    using add_cv_t = typename add_cv<T>::type;

    template<class T>
    struct remove_cvref {
        using type = std::remove_cv_t<std::remove_reference_t<T>>;
    };
    template <class T>
    using remove_cvref_t = typename remove_cvref<T>::type;


    //===============================================================
    // 参照の変更
    //===============================================================
    using std::remove_reference;
    template <class T>
    using remove_reference_t = typename remove_reference<T>::type;

    using std::add_lvalue_reference;
    template <class T>
    using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

    using std::add_rvalue_reference;
    template <class T>
    using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;


    //===============================================================
    // 符号の変更
    //===============================================================
    using std::make_signed;
    template <class T>
    using make_signed_t = typename make_signed<T>::type;

    using std::make_unsigned;
    template <class T>
    using make_unsigned_t = typename make_unsigned<T>::type;


    //===============================================================
    // 配列の変更
    //===============================================================
    using std::remove_extent;
    template <class T>
    using remove_extent_t = typename remove_extent<T>::type;

    using std::remove_all_extents;
    template <class T>
    using remove_all_extents_t = typename remove_all_extents<T>::type;


    //===============================================================
    // ポインタの変更
    //===============================================================
    using std::add_pointer;
    template <class T>
    using add_pointer_t = typename add_pointer<T>::type;

    using std::remove_pointer;
    template <class T>
    using remove_pointer_t = typename remove_pointer<T>::type;


    //===============================================================
    // 関数呼び出しに関連した特性
    //===============================================================
    using std::is_invocable;
    template <class F, class... ArgsTypes>
    inline constexpr bool is_invocable_v = std::is_invocable<F, ArgsTypes...>::value;

    using std::is_invocable_r;
    template <class R, class F, class... ArgsTypes>
    inline constexpr bool is_invocable_r_v = std::is_invocable_r<R, F, ArgsTypes...>::value;

    using std::is_nothrow_invocable;
    template <class F, class... ArgsTypes>
    inline constexpr bool is_nothrow_invocable_v = std::is_nothrow_invocable<F, ArgsTypes...>::value;

    using std::is_nothrow_invocable_r;
    template <class R, class F, class... ArgsTypes>
    inline constexpr bool is_nothrow_invocable_r_v = std::is_nothrow_invocable_r<R, F, ArgsTypes...>::value;


    //===============================================================
    // その他の変換
    //===============================================================
    using std::decay;
    template <class T>
    using decay_t = typename decay<T>::type;

    using std::enable_if;
    template <bool Condition, class T = void>
    using enable_if_t = typename enable_if<Condition, T>::type;

    using std::conditional;
    template <bool B, class T, class F>
    using conditional_t = typename conditional<B, T, F>::type;

    using std::common_type;
    template <class... T>
    using common_type_t = typename common_type<T...>::type;

    using std::void_t;

    using std::underlying_type;
    template <class T>
    using underlying_type_t = typename underlying_type<T>::type;

    using std::invoke_result;
    template <class F, class... ArgsTypes>
    using invoke_result_t = typename invoke_result<F, ArgsTypes...>::type;


    //===============================================================
    // 論理演算
    //===============================================================
    using std::conjunction;
    template <class... Traits>
    inline constexpr bool conjunction_v = conjunction<Traits...>::value;

    using std::disjunction;
    template <class... Traits>
    inline constexpr bool disjunction_v = disjunction<Traits...>::value;

    using std::negation;
    template <class Trait>
    inline constexpr bool negation_v = negation<Trait>::value;


}// namespcae ob
//!@endcond