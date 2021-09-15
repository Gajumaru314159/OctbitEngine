//***********************************************************
//! @file
//! @brief		キュー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Template/allocator.h>
#include <queue>

namespace ob
{

    template <class T>
    using queue = std::queue<T, allocator<T>>;
    template <class T>
    using priority_queue = std::priority_queue<T, allocator<T>>;

}// namespcae ob