//***********************************************************
//! @file
//! @brief		メモリー
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタの呼び出し
    //! 
    //! @details        呼び出しはplacement newによって行われる。
    //! @param p        インスタンスのポインタ
    //! @param ...args  コンストラクタの引数
    //! @return         インスタンスのポインタ
    //@―---------------------------------------------------------------------------
    template<class T, class... Args>
    constexpr T* construct_at(T* p, Args&&... args) {
        return ::new (const_cast<void*>(static_cast<const volatile void*>(p)))
            T(ob::forward<Args>(args)...);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          範囲の各要素に対してコンストラクタの呼び出し
    //! 
    //! @details        呼び出しはplacement newによって行われる。
    //! @param first    開始イテレータ
    //! @param last     終了イテレータ
    //! @param ...args  コンストラクタの引数
    //@―---------------------------------------------------------------------------
    template<class T, class ForwardIterator, class... Args>
    constexpr void construct(ForwardIterator first, ForwardIterator last, Args&&... args) {
        for (; first != last; ++first)
            ob::construct_at(&(*first), ob::forward(args)...);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 範囲のうち先頭 N この要素に対してコンストラクタの呼び出し
    //! 
    //! @details        呼び出しはplacement newによって行われる。
    //! @param first    開始イテレータ
    //! @param n        要素数
    //! @param ...args  コンストラクタの引数
    //@―---------------------------------------------------------------------------
    template< class ForwardIt, class Size, class... Args>
    constexpr ForwardIt destroy_n(ForwardIt first, Size n, Args&&... args) {
        for (; n > 0; (void) ++first, --n)
            ob::construct_at(&(*first));
    }


}// namespcae ob