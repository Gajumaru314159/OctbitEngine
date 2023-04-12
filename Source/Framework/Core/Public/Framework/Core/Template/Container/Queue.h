//***********************************************************
//! @file
//! @brief		キュー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <queue>
#include <Framework/Core/Template/Allocator/STLAllocator.h>
#include <Framework/Core/Template/Container/Deque.h>
#include <Framework/Core/Template/Container/Array.h>

namespace ob::core {

    //! @brief キュー
    template <class T, class TConatiner = Deque<T>>
    using Queue = std::queue<T, TConatiner>;

    //! @brief 優先度付きキュー
    template <class T, class TContainer = Array<T>, class TPr = std::less<typename TContainer::value_type>>
    using PriorityQueue = std::priority_queue<T,TContainer,TPr>;

}