//***********************************************************
//! @file
//! @brief		スタック
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <stack>
#include "../allocator.h"

namespace ob::foundation {

    //! @brief スタック
    template <class T>
    using stack = std::stack<T>;

}// namespcae ob