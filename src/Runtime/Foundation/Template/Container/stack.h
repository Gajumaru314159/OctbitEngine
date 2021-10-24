//***********************************************************
//! @file
//! @brief		スタック
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Foundation/Template/allocator.h>
#include <stack>

namespace ob
{

    template <class T>
    using stack = std::stack<T, allocator<T>>;

}// namespcae ob