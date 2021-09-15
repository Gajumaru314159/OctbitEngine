//***********************************************************
//! @file
//! @brief		連想コンテナ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Template/allocator.h>
#include <map>
#include <unordered_map>

namespace ob
{

    template <class TKey,class TValue>
    using map = std::map<TKey,TValue, allocator<pair<const TKey, TPair>>>;
    template <class TKey, class TValue>
    using unordered_map = std::unordered_map<TKey, TValue, allocator<pair<const TKey, TPair>>>;
    template <class TKey, class TValue>
    using multimap = std::multimap<TKey, TValue, allocator<pair<const TKey, TPair>>>;
    template <class TKey, class TValue>
    using unordered_multimap = std::unordered_multimap<TKey, TValue, allocator<pair<const TKey, TPair>>>;

}// namespcae ob