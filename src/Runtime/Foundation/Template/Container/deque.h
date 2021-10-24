//***********************************************************
//! @file
//! @brief		Deque(二重終端キュー)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Foundation/Template/allocator.h>
#include <deque>

namespace ob
{

    template <class T>
    using deque = std::deque<T,allocator<T>>;

}// namespcae ob