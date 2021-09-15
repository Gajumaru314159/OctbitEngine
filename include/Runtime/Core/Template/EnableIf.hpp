//***********************************************************
//! @file
//! @brief		Ifテンプレート
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob
{

    //-----------------------------------------------------------
    //! @brief              コンパイル時条件式が真の場合のみ有効な型
    //! 
    //! @see                https://cpprefjp.github.io/reference/type_traits/enable_if.html
    //! @tparam Condition   コンパイル時条件式 
    //! @tparam T           型
    //-----------------------------------------------------------
    template <bool Condition, typename T = void>
    class EnableIf;

    //! @cond 
    template <typename T>
    class EnableIf<true, T>
    {
    public:
        typedef T Type;
    };

    template <typename T>
    class EnableIf<false, T>
    { };
    //! @endcond


}// namespcae ob