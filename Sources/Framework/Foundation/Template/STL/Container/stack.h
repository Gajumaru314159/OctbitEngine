//***********************************************************
//! @file
//! @brief		スタック
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <stack>
#include "../allocator.h"

namespace ob::foundation {

    template <class T>
    using stack = std::stack<T, allocator<T>>;

}// namespcae ob