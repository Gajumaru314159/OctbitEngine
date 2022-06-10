//***********************************************************
//! @file
//! @brief		スタック
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <stack>
#include <Framework/Core/Template/STLAllocator.h>
#include <Framework/Core/Template/Container/Deque.h>

namespace ob::core {

    //! @brief スタック
    template <class T>
    using stack = std::stack<T,ob::core::deque<T>>;

}// namespcae ob