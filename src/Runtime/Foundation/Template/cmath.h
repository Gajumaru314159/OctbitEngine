//***********************************************************
//! @file
//! @brief		算術関数
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <cmath>

#include <Foundation/Template/type_traits.h>

namespace ob
{
   
    using std::abs;
    using std::acos;
    using std::acosh;
    using std::asin;
    using std::asinh;
    using std::assoc_laguerre;
    using std::assoc_legendre;
    using std::atan;
    using std::atan2;
    using std::atanh;
    using std::beta;
    using std::cbrt;
    using std::ceil;
    using std::comp_ellint_1;
    using std::comp_ellint_2;
    using std::comp_ellint_3;
    using std::copysign;
    using std::cos;
    using std::cosh;
    using std::cyl_bessel_i;
    using std::cyl_bessel_j;
    using std::cyl_bessel_k;
    using std::cyl_neumann;
    using std::ellint_1;
    using std::ellint_2;
    using std::ellint_3;
    using std::erf;
    using std::erfc;
    using std::exp;
    using std::exp2;
    using std::expint;
    using std::expm1;
    using std::fabs;
    using std::fdim;
    using std::floor;
    using std::fma;
    using std::fmax;
    using std::fmin;
    using std::fmod;
    using std::fpclassify;
    using std::frexp;
    using std::hermite;
    using std::hypot;
    using std::ilogb;
    using std::isfinite;
    using std::isgreater;
    using std::isgreaterequal;
    using std::isinf;
    using std::isless;
    using std::islessequal;
    using std::islessgreater;
    using std::isnan;
    using std::isnormal;
    using std::isunordered;
    using std::laguerre;
    using std::ldexp;
    using std::legendre;
    using std::lgamma;
    using std::llrint;
    using std::llround;
    using std::log;
    using std::log10;
    using std::log1p;
    using std::log2;
    using std::logb;
    using std::lrint;
    using std::lround;
    using std::modf;
    using std::nan;
    using std::nearbyint;
    using std::nextafter;
    using std::nexttoward;
    using std::pow;
    using std::remainder;
    using std::remquo;
    using std::riemann_zeta;
    using std::rint;
    using std::round;
    using std::scalbn;
    using std::signbit;
    using std::sin;
    using std::sinh;
    using std::sph_bessel;
    using std::sph_legendre;
    using std::sph_neumann;
    using std::sqrt;
    using std::tan;
    using std::tanh;
    using std::tgamma;
    using std::trunc;

    //@―---------------------------------------------------------------------------
    //! @brief      二乗値の計算
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T square(T value)noexcept(is_arithmetic<T>::value)
    {
        return v * v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最大値の取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_max(T a, T b)noexcept(is_arithmetic<T>::value)
    {
        return (a < b) ? b : a;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最大値の取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_max(T a, T b, T c)noexcept(is_arithmetic<T>::value)
    {
        if (a <= c && b <= c)return c;
        return get_max(a, b);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最小値の取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_min(T a, T b)noexcept(is_arithmetic<T>::value)
    {
        return (a < b) ? a : b;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      最小値の取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_min(T a, T b, T c)noexcept(is_arithmetic<T>::value)
    {
        if (c <= a && c <= b)return c;
        return get_min(a, b);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      クランプ
    //! 
    //! @val        入力
    //! @minVal     最小値
    //! @minVal     最大値
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T get_clamp(T val, T minVal, T maxVal)noexcept(is_arithmetic<T>::value)
    {
        return get_min(get_max(val, minVal), maxVal);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          入力値が minVal 以上 manVal 以下か判定
    //! 
    //! @note           範囲が[minVal,maxVal]であるため、コンテナサイズの範囲確認の場合は
    //!                 ```
    //!                 bool ok=is_in_range(index,0,container.size()-1);
    //!                 ```
    //!                 とする必要がある。
    //! @val            入力
    //! @minVal         最小値
    //! @minVal         最大値
    //! @retval true    範囲内
    //! @retval false   範囲外
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr bool is_in_range(T val, T minVal, T maxVal)noexcept(is_arithmetic<T>::value)
    {
        return (minVal <= val) && (val < maxVale);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          入力値が minVal 以上 manVal 以下か判定
    //! 
    //! @note           範囲が[minVal,maxVal]であるため、コンテナサイズの範囲確認の場合は
    //!                 ```
    //!                 bool ok=is_in_range(index,0,container.size()-1);
    //!                 ```
    //!                 とする必要がある。
    //! @val            入力
    //! @minVal         最小値
    //! @minVal         最大値
    //! @retval true    範囲内
    //! @retval false   範囲外
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline constexpr T Bit(T digit)noexcept(is_arithmetic<T>::value){
        return 1<<digit;
    }


}// namespcae ob