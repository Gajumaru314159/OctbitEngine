//***********************************************************
//! @file
//! @brief		キュー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <queue>
#include "../allocator.h"

namespace ob::core {

    //! @brief キュー
    template <class T>
    using queue = std::queue<T, deque<T>>;

    //! @brief 優先度付きキュー
    template <class T>
    using priority_queue = std::priority_queue<T>;

}// namespcae ob