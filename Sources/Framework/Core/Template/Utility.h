﻿//***********************************************************
//! @file
//! @brief		ユーティリティ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>
#include <type_traits>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief      enum値を基底型(整数型)に変換する
    //! 
    //! @param e    enum値
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr auto enum_cast(T e)->std::enable_if_t<std::is_enum_v<T>, std::underlying_type_t<T>> {
        return static_cast<underlying_type_t<T>>(e);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンテナのサイズを取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr s32 get_size(const T& container)noexcept(noexcept(container.size())) {
        return static_cast<s32>(container.size());
    }


    //@―---------------------------------------------------------------------------
    //! @brief      静的配列のサイズを取得
    //@―---------------------------------------------------------------------------
    template<typename T, s32 N>
    inline constexpr s32 get_size(const T(&)[N])noexcept {
        return N;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      二乗値の計算
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_square(T value)noexcept(std::is_arithmetic<T>::value) {
        return v * v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最大値を取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_max(T a, T b)noexcept(std::is_arithmetic<T>::value) {
        return (a < b) ? b : a;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最大値を取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_max(T a, T b, T c)noexcept(std::is_arithmetic<T>::value) {
        if (a <= c && b <= c)return c;
        return get_max(a, b);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最小値を取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_min(T a, T b)noexcept(std::is_arithmetic<T>::value) {
        return (a < b) ? a : b;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最小値を取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_min(T a, T b, T c)noexcept(std::is_arithmetic<T>::value) {
        if (c <= a && c <= b)return c;
        return get_min(a, b);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      クランプ
    //! 
    //! @param val        入力
    //! @param minVal     最小値
    //! @param maxVal     最大値
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_clamp(T val, T minVal, T maxVal)noexcept(std::is_arithmetic<T>::value) {
        return get_min(get_max(val, minVal), maxVal);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      0～1にクランプ
    //! 
    //! @param val        入力
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_clamp01(T val)noexcept(std::is_arithmetic<T>::value) {
        return get_clamp(val, (T)0, (T)1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          入力値が minVal 以上 manVal 以下か判定
    //! 
    //! @param val      入力
    //! @param minVal   最小値
    //! @param maxVal   最大値
    //! @retval true    範囲内
    //! @retval false   範囲外
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr bool is_in_range(T val, T minVal, T maxVal)noexcept(std::is_arithmetic<T>::value) {
        return (minVal <= val) && (val <= maxVal);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          インデックスがコンテナ内か判定
    //! 
    //! @param index    インデックス
    //! @param container コンテナ
    //! @retval true    範囲内
    //! @retval false   範囲外
    //@―---------------------------------------------------------------------------
    template<typename T, typename TContainer>
    inline constexpr bool is_in_range(T index, const TContainer& container)noexcept(std::is_arithmetic<T>::value) {
        return is_in_range<T>(index, (T)0, (T)container.size());
    }


    //@―---------------------------------------------------------------------------
    //! @brief          1<<digit
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_bit(T digit)noexcept(std::is_arithmetic<T>::value) {
        return 1 << digit;
    }


    //@―---------------------------------------------------------------------------
    //! @brief                  値を一番近いアライメントの倍数に増やして合わせる
    //!
    //! @param val              位置合わせするアライメント値
    //! @param alignment        アライメントの値(2の累乗)
    //! @return                 アラインされた値
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T align_up(T val, size_t alignment) {
        return (T)(((size_t)val + alignment - 1) & (~(alignment - 1)));
    }


    //@―---------------------------------------------------------------------------
    //! @brief                  値を一番近いアライメントの倍数に減らして合わせる
    //!
    //! @param val              位置合わせするアライメント値
    //! @param alignment        アライメントの値(2の累乗)
    //! @return                 アラインされた値
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T align_down(T val, size_t alignment) {
        return (T)(((size_t)val) & (~(alignment - 1)));
    }


    //@―---------------------------------------------------------------------------
    //! @brief                  値を一番近いアライメントの倍数に合わせる
    //!
    //! @param val              位置合わせするアライメント値
    //! @param alignment        アライメントの値(2の累乗)
    //! @return                 アラインされた値
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T align_near(T val, size_t alignment) {
        return (T)((((size_t)val + alignment - 1) / alignment) * alignment);
    }


    //@―---------------------------------------------------------------------------
    //! @brief                  値がアライメントの倍数か判定
    //!
    //! @param val              判定する値
    //! @param alignment        アライメントの値(2の累乗)
    //! @retval TRUE            アライメントされている
    //! @retval FALSE           アライメントされていない
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr bool is_aligned(T val, size_t alignment) {
        return !((size_t)val & (alignment - 1));
    }

}// namespcae ob