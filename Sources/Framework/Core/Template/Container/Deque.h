//***********************************************************
//! @file
//! @brief		Deque(二重終端キュー)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <deque>
#include <Framework/Core/Template/STLAllocator.h>

namespace ob::core {

    //! @brief デック
    template <class T>
    using deque = std::deque<T, ob::core::allocator<T>>;

}// namespcae ob