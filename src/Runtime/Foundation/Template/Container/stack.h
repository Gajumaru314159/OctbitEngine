//***********************************************************
//! @file
//! @brief		スタック
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Template/allocator.h>
#include <stack>

namespace ob
{

    template <class T>
    using stack = std::stack<T, allocator<T>>;

}// namespcae ob