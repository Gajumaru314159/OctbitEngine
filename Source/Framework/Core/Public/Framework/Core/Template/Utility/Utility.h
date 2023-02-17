//***********************************************************
//! @file
//! @brief		ユーティリティ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>
#include <type_traits>
#include <algorithm>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief      enum値を基底型(整数型)に変換する
    //! 
    //! @param e    enum値
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr auto enum_cast(T e)->std::enable_if_t<std::is_enum_v<T>, std::underlying_type_t<T>> {
        return static_cast<std::underlying_type_t<T>>(e);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      二乗値の計算
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_square(T value)noexcept(std::is_arithmetic<T>::value) {
        return v * v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      0～1にクランプ
    //! 
    //! @param val        入力
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T clamp01(T val)noexcept(std::is_arithmetic<T>::value) {
        return std::clamp(val, (T)0, (T)1);
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
        return is_in_range<T>(index, (T)0, (T)container.size()-1);
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


    //@―---------------------------------------------------------------------------
    //! @brief                  値が大きい場合のみ値を更新する
    //! 
    //! @param out              出力先
    //! @param val              比較値
    //! @retval true            更新あり
    //! @retval false           更新なし
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr bool update_max(T& out, T val)noexcept(std::is_arithmetic<T>::value) {
        if (out < val)
            return false;

        out = val;

        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief                  値が小さい場合のみ値を更新する
    //! 
    //! @param out              出力先
    //! @param val              比較値
    //! @retval true            更新あり
    //! @retval false           更新なし
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr bool update_min(T& out, T val)noexcept(std::is_arithmetic<T>::value) {
        if (out > val)
            return false;

        out = val;

        return true;
    }

}// namespcae ob