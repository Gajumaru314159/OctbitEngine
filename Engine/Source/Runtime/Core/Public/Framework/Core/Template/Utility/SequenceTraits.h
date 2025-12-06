//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <type_traits>
#include <Framework/Core/Template/Container/Array.h>
#include <Framework/Core/Template/Container/Vector.h>
#include <Framework/Core/Template/Container/List.h>
#include <Framework/Core/Template/Container/Map.h>
#include <Framework/Core/Template/Container/Set.h>

namespace ob::core {

	template < typename T >
	struct is_sequence : std::false_type {};

	template<typename T, typename ALLOCATOR>
	struct is_sequence<List<T, ALLOCATOR>> : std::true_type {};

	template<typename T, typename ALLOCATOR>
	struct is_sequence<Set<T, ALLOCATOR>> : std::true_type {};

	template<typename T, typename ALLOCATOR>
	struct is_sequence<HashSet<T, ALLOCATOR>> : std::true_type {};

	template<typename T, typename ALLOCATOR>
	struct is_sequence<Vector<T, ALLOCATOR>> : std::true_type {};

	template<typename T, size_t N>
	struct is_sequence<Array<T, N>> : std::true_type {};

	template<typename T, size_t N>
	struct is_sequence<T[N]> : std::true_type {};


	template < typename T >
	struct is_map : std::false_type {};

	template<typename TKey, class TValue>
	struct is_map<Map<TKey, TValue>> : std::true_type {};

	template<typename TKey, class TValue>
	struct is_map<HashMap<TKey, TValue>> : std::true_type {};

}