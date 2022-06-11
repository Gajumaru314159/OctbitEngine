//***********************************************************
//! @file
//! @brief		連想コンテナ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <map>
#include <unordered_map>
#include <type_traits>
#include <Framework/Core/Template/Allocator/STLAllocator.h>

namespace ob::core {
    
    //! @brief 辞書配列
    template <class TKey, class TValue, class TCompare = std::less<TKey>, class TAlloc = ob::core::allocator<std::pair<const TKey,TValue>>>
    using Map = std::map<TKey, TValue, TCompare, TAlloc>;

    //! @brief ハッシュ辞書配列
    template <class TKey, class TValue, class TAlloc = ob::core::allocator<std::pair<const TKey, TValue>>>
    using HashMap = std::unordered_map<TKey, TAlloc>;

    //! @brief 複数辞書配列
    template <class TKey, class TValue, class TCompare = std::less<TKey>, class TAlloc = ob::core::allocator<std::pair<const TKey, TValue>>>
    using MultiMap = std::multimap<TKey, TValue, TCompare, TAlloc>;

    //! @brief 複数ハッシュ辞書配列
    template <class TKey, class TValue, class TAlloc = ob::core::allocator<std::pair<const TKey, TValue>>>
    using HashMultiMap = std::unordered_multimap<TKey, TValue, TAlloc>;

}// namespcae ob