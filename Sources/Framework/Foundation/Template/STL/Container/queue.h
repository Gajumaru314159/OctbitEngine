//***********************************************************
//! @file
//! @brief		キュー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <queue>
#include "../allocator.h"

namespace ob::foundation {

    template <class T>
    using queue = std::queue<T, deque<T>>;
    template <class T>
    using priority_queue = std::priority_queue<T>;

}// namespcae ob