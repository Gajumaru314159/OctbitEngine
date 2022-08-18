//***********************************************************
//! @file
//! @brief		動的配列
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <vector>
#include <Framework/Core/Template/Allocator/STLAllocator.h>

namespace ob::core {

	//! @brief 動的配列
	template <class T, class TAlloc = ob::core::STLAllocator<T>>
	using Array = std::vector<T, TAlloc>;

}// namespcae ob