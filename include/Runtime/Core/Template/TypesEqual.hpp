//***********************************************************
//! @file
//! @brief		型チェックテンプレート
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob
{
   
    //-----------------------------------------------------------
    //! @brief 型比較テンプレート
    //! 
    //! @details TypesEqual<A,B>::ValueがTRUEなら同じ型
    //! @tparam A 型A
    //! @tparam B 型B
    //-----------------------------------------------------------
    template<typename A, typename B>
    struct TypesEqual;

    //! @cond  
    template<typename, typename>
    struct TypesEqual
    {
        enum { Value = false };
    };

    template<typename A>
    struct TypesEqual<A, A>
    {
        enum { Value = true };
    };
    //! @endcond  


    //-----------------------------------------------------------
    //! @brief 型比較マクロ
    //-----------------------------------------------------------
#define OB_TYPES_EQUAL(A,B) TypesEqual<A,B>::Value

}// namespcae ob