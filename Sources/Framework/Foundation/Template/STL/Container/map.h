﻿//***********************************************************
//! @file
//! @brief		連想コンテナ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <map>
#include <unordered_map>
#include <Framework/Foundation/Template/STL/type_traits.h>
#include "../allocator.h"

namespace ob::foundation {

    template <class TKey, class TValue,class TCompare=less<TKey>>
    using map = std::map<TKey, TValue, TCompare, allocator<pair<const TKey, TValue>>>;

    template <class TKey, class TValue>
    using unordered_map = std::unordered_map<TKey, TValue, allocator<pair<const TKey, TValue>>>;

    template <class TKey, class TValue, class TCompare = less<TKey>>
    using multimap = std::multimap<TKey, TValue, TCompare, allocator<pair<const TKey, TValue>>>;

    template <class TKey, class TValue>
    using unordered_multimap = std::unordered_multimap<TKey, TValue, allocator<pair<const TKey, TValue>>>;

}// namespcae ob