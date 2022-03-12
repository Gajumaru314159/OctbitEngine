//***********************************************************
//! @file
//! @brief		ユーティリティ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::foundation {

    //@―---------------------------------------------------------------------------
    //! @brief      enum値を基底型(整数型)に変換する
    //! 
    //! @param e    enum値
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr auto enum_cast(T e)->enable_if_t<is_enum_v<T>, underlying_type_t<T>> {
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
    inline constexpr T get_square(T value)noexcept(is_arithmetic<T>::value) {
        return v * v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最大値の取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_max(T a, T b)noexcept(is_arithmetic<T>::value) {
        return (a < b) ? b : a;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最大値の取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_max(T a, T b, T c)noexcept(is_arithmetic<T>::value) {
        if (a <= c && b <= c)return c;
        return get_max(a, b);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最小値の取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_min(T a, T b)noexcept(is_arithmetic<T>::value) {
        return (a < b) ? a : b;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最小値の取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_min(T a, T b, T c)noexcept(is_arithmetic<T>::value) {
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
    inline constexpr T get_clamp(T val, T minVal, T maxVal)noexcept(is_arithmetic<T>::value) {
        return get_min(get_max(val, minVal), maxVal);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      0～1にクランプ
    //! 
    //! @param val        入力
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_clamp01(T val)noexcept(is_arithmetic<T>::value) {
        return get_clamp(val, (T)0, (T)1);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          入力値が minVal 以上 manVal 以下か判定
    //! 
    //! @note           範囲が[minVal,maxVal]であるため、コンテナサイズの範囲確認の場合は
    //!                 ```
    //!                 bool ok=is_in_range(index,0,container.size()-1);
    //!                 ```
    //!                 とする必要がある。
    //! @param val      入力
    //! @param minVal   最小値
    //! @param maxVal   最大値
    //! @retval true    範囲内
    //! @retval false   範囲外
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr bool is_in_range(T val, T minVal, T maxVal)noexcept(is_arithmetic<T>::value) {
        return (minVal <= val) && (val < maxVal);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          入力値が minVal 以上 manVal 以下か判定
    //! 
    //! @note           範囲が[minVal,maxVal]であるため、コンテナサイズの範囲確認の場合は
    //!                 ```
    //!                 bool ok=is_in_range(index,0,container.size()-1);
    //!                 ```
    //!                 とする必要がある。
    //! @param index    インデックス
    //! @param container コンテナ
    //! @retval true    範囲内
    //! @retval false   範囲外
    //@―---------------------------------------------------------------------------
    template<typename T, typename TContainer>
    inline constexpr bool is_in_range(T index, const TContainer& container)noexcept(is_arithmetic<T>::value) {
        return is_in_range<T>(index, 0, container.size());
    }


    //@―---------------------------------------------------------------------------
    //! @brief          1<<digit
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_bit(T digit)noexcept(is_arithmetic<T>::value) {
        return 1 << digit;
    }

}// namespcae ob