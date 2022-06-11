//***********************************************************
//! @file
//! @brief		セット(集合)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <set>
#include <unordered_set>
#include <Framework/Core/Template/Allocator/STLAllocator.h>

namespace ob::core {

	//! @brief 順序あり集合
	template <class T, class TPr = std::less<T>, class TAlloc = ob::core::allocator<T>>
	using Set = std::set<T, TPr, TAlloc>;

	//! @brief 集合
	template <class T, class THasher = std::hash<T>, class TKeyEq = std::equal_to<T>, class TAlloc = ob::core::allocator<T>>
	using HashSet = std::unordered_set<T, THasher, TKeyEq, TAlloc>;

	//! @brief 複数順序あり集合
	template <class T, class TPr = std::less<T>, class TAlloc = ob::core::allocator<T>>
	using MultiSet = std::multiset<T, TPr, TAlloc>;

	//! @brief 複数集合
	template <class T, class THasher = std::hash<T>, class TKeyEq = std::equal_to<T>, class TAlloc = ob::core::allocator<T>>
	using HashMultiSet = std::unordered_multiset<T, THasher, TKeyEq, TAlloc>;

}// namespcae ob