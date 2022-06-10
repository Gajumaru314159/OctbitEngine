//***********************************************************
//! @file
//! @brief		可変長配列
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <vector>
#include <Framework/Core/Template/Container/Deque.h>

namespace ob::core {

    //! @brief 動的配列
    template <class T>
    using vector = std::vector<T, ob::core::allocator<T>>;

}// namespcae ob