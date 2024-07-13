//***********************************************************
//! @file
//! @brief		バリアント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <type_traits>
#include <Framework/Core/Template/Container/Array.h>
#include <Framework/Core/Template/Container/Vector.h>

namespace ob::core {

	template < typename T >
	struct is_sequence : std::false_type {};

	template<typename T, typename ALLOCATOR>
	struct is_sequence<Vector<T, ALLOCATOR>> : std::true_type {};

	template<typename T, size_t N>
	struct is_sequence<Array<T, N>> : std::true_type {};

	template<typename T, size_t N>
	struct is_sequence<T[N]> : std::true_type {};

}