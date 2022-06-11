//***********************************************************
//! @file
//! @brief		Deque(二重終端キュー)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <deque>
#include <Framework/Core/Template/Allocator/STLAllocator.h>

namespace ob::core {

    //! @brief Deque(二重終端キュー)
    template <class T, class TAlloc = ob::core::STLAllocator<T>>
    using Deque = std::deque<T, TAlloc>;

}// namespcae ob