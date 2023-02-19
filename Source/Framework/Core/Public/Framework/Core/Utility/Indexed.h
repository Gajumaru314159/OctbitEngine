//***********************************************************
//! @file
//! @brief		インデックス付きEnumlator
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  インデックス付きEnumlator
    //@―---------------------------------------------------------------------------
	template <class Type, class TItr = decltype(std::begin(std::declval<Type>())), class = decltype(std::end(std::declval<Type>()))>
	inline constexpr auto IndexedRef(Type&& iterable)
	{
		// Reference: http://reedbeta.com/blog/python-like-enumerate-in-cpp17/
		// Reference: https://github.com/Siv3D/OpenSiv3D/blob/main/Siv3D/include/Siv3D/Indexed.hpp

		struct Iterator
		{
			size_t index;
			TItr itr;
			constexpr bool operator != (const Iterator& other) const { return itr != other.itr; }
			constexpr void operator ++() { ++index; ++itr; }
			constexpr auto operator *() const { return std::tie(index, *itr); }
		};

		struct IterableWrapper
		{
			Type iterable;
			constexpr auto begin() { return Iterator{ 0, std::begin(iterable) }; }
			constexpr auto end() { return Iterator{ 0, std::end(iterable) }; }
		};

		return IterableWrapper{ std::forward<Type>(iterable) };
	}

	//@―---------------------------------------------------------------------------
	//! @brief		インデックス付きEnumlator
	//! 
	//! @details	使用方法
	//!				```c++
	//!				for(auto[index,item]:Indexed(container)){
	//! 
	//!				}
	//!				```
	//@―---------------------------------------------------------------------------
	template <class Type, class TItr = decltype(std::begin(std::declval<Type>())), class = decltype(std::end(std::declval<Type>()))>
	inline constexpr auto Indexed(Type&& iterable)
	{
		return IndexedRef<Type, TItr>(std::forward<Type>(iterable));
	}

	//@―---------------------------------------------------------------------------
	//! @brief  インデックス付き逆Enumlator
	//@―---------------------------------------------------------------------------
	template <class Type, class TItr = decltype(std::rbegin(std::declval<Type>())), class = decltype(std::rend(std::declval<Type>()))>
	inline constexpr auto ReverseIndexedRef(Type&& iterable)
	{
		struct Iterator
		{
			size_t index;
			TItr itr;
			constexpr bool operator != (const Iterator& other) const { return itr != other.itr; }
			constexpr void operator ++() { --index; ++itr; }
			constexpr auto operator *() const { return std::tie(index, *itr); }
		};

		struct IterableWrapper
		{
			Type iterable;
			constexpr auto begin() { return Iterator{ std::size(iterable) - 1, std::rbegin(iterable) }; }
			constexpr auto end() { return Iterator{ 0, std::rend(iterable) }; }
		};

		return IterableWrapper{ std::forward<Type>(iterable) };
	}

	//@―---------------------------------------------------------------------------
	//! @brief  インデックス付き逆Enumlator
	//@―---------------------------------------------------------------------------
	template <class Type, class TItr = decltype(std::rbegin(std::declval<Type>())), class = decltype(std::rend(std::declval<Type>()))>
	inline constexpr auto ReverseIndexed(Type&& iterable)
	{
		return ReverseIndexedRef<Type, TItr>(std::forward<Type>(iterable));
	}


}// namespcae ob