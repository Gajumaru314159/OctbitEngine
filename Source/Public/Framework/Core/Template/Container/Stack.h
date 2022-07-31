//***********************************************************
//! @file
//! @brief		スタック
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <stack>
#include <Framework/Core/Template/Allocator/STLAllocator.h>
#include <Framework/Core/Template/Container/Deque.h>

namespace ob::core {

    //! @brief スタック
    template <class T,class TContainer = Deque<T>>
    using Stack = std::stack<T, TContainer>;

}// namespcae ob