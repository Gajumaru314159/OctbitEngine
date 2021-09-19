//***********************************************************
//! @file
//! @brief		算術関数
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Template/type_traits.h>
namespace ob
{

    //@―-------------------------------------------------------------------------------
    //! @brief  2乗の計算
    //@―-------------------------------------------------------------------------------
    template<typename T>
    inline auto square(T value)->typename enable_if<is_arithmetic<T>::value>::type
    {
        return value * value;
    }


    //@―-------------------------------------------------------------------------------
    //! @brief  大きいほうの取得
    //@―-------------------------------------------------------------------------------
    template<typename T>
    inline T get_max(T a, T b)
    {
        return ((a < b) ? b : a);
    }


    //@―-------------------------------------------------------------------------------
    //! @brief  小さいほうの取得
    //@―-------------------------------------------------------------------------------
    template<typename T>
    inline T get_min(T a, T b)
    {
        return ((a < b) ? a : b);
    }


    //@―-------------------------------------------------------------------------------
    //! @brief  クランプ
    //@―-------------------------------------------------------------------------------
    template<typename T>
    inline auto get_clamp(T val, T minVal, T maxVal)->typename enable_if<is_arithmetic<T>::value>::type
    {
        return get_max(get_min(val, maxVal), minVal);
    }


    //@―-------------------------------------------------------------------------------
    //! @brief  クランプ
    //@―-------------------------------------------------------------------------------
    template<typename T>
    inline auto is_in_range(T val, T minVal, T maxVal)->typename enable_if<is_arithmetic<T>::value>::type
    {
        return (minVal<=val)&&T(val<maxVal);
    }

}// namespcae ob