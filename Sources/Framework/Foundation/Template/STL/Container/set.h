//***********************************************************
//! @file
//! @brief		セット(集合)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <set>
#include <unordered_set>
#include "../allocator.h"

namespace ob::foundation {

    //! @brief 順序あり集合
    template <class T>
    using set = std::set<T, less<T>, allocator<T>>;

    //! @brief 集合
    template <class T>
    using unordered_set = std::unordered_set<T,hash<T>, equal_to<T>, allocator<T>>;

    //! @brief 複数順序あり集合
    template <class T>
    using multiset = std::multiset<T, less<T>, allocator<T>>;

    //! @brief 複数集合
    template <class T>
    using unordered_multiset = std::unordered_multiset<T, hash<T>, equal_to<T>, allocator<T>>;

}// namespcae ob