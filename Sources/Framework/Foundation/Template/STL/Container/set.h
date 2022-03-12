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

    template <class T>
    using set = std::set<T, allocator<T>>;
    template <class T>
    using unordered_set = std::unordered_set<T, allocator<T>>;
    template <class T>
    using multiset = std::multiset<T, allocator<T>>;
    template <class T>
    using unordered_multiset = std::unordered_multiset<T, allocator<T>>;

}// namespcae ob