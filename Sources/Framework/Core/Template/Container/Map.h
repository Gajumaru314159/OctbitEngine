//***********************************************************
//! @file
//! @brief		連想コンテナ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <map>
#include <unordered_map>
#include <type_traits>
#include <Framework/Core/Template/STLAllocator.h>

namespace ob::core {

    //! @brief 辞書配列
    template <class TKey, class TValue,class TCompare=std::less<TKey>>
    using map = std::map<TKey, TValue, TCompare, ob::core::allocator<std::pair<const TKey, TValue>>>;

    //! @brief ハッシュ辞書配列
    template <class TKey, class TValue>
    using unordered_map = std::unordered_map<TKey, TValue, ob::core::allocator<std::pair<const TKey, TValue>>>;

    //! @brief 複数辞書配列
    template <class TKey, class TValue, class TCompare = std::less<TKey>>
    using multimap = std::multimap<TKey, TValue, TCompare, ob::core::allocator<std::pair<const TKey, TValue>>>;

    //! @brief 複数ハッシュ辞書配列
    template <class TKey, class TValue>
    using unordered_multimap = std::unordered_multimap<TKey, TValue, ob::core::allocator<std::pair<const TKey, TValue>>>;

}// namespcae ob