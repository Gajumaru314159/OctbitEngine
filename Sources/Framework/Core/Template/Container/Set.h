//***********************************************************
//! @file
//! @brief		セット(集合)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <set>
#include <unordered_set>
#include <Framework/Core/Template/STLAllocator.h>

namespace ob::core {

    //! @brief 順序あり集合
    template <class T>
    using set = std::set<T, std::less<T>, ob::core::allocator<T>>;

    //! @brief 集合
    template <class T>
    using unordered_set = std::unordered_set<T,std::hash<T>, std::equal_to<T>, ob::core::allocator<T>>;

    //! @brief 複数順序あり集合
    template <class T>
    using multiset = std::multiset<T, std::less<T>, ob::core::allocator<T>>;

    //! @brief 複数集合
    template <class T>
    using unordered_multiset = std::unordered_multiset<T, std::hash<T>, std::equal_to<T>, ob::core::allocator<T>>;

}// namespcae ob