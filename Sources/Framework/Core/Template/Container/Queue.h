//***********************************************************
//! @file
//! @brief		キュー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <queue>
#include <Framework/Core/Template/STLAllocator.h>
#include <Framework/Core/Template/Container/Deque.h>

namespace ob::core {

    //! @brief キュー
    template <class T>
    using queue = std::queue<T, ob::core::deque<T>>;

    //! @brief 優先度付きキュー
    template <class T>
    using priority_queue = std::priority_queue<T>;

}// namespcae ob