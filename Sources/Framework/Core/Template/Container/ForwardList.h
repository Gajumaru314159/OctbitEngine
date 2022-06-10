//***********************************************************
//! @file
//! @brief		単方向リスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <forward_list>
#include <Framework/Core/Template/STLAllocator.h>

namespace ob::core {

    //! @brief 単連結リスト
    template <class T>
    using forward_list = std::forward_list<T, ob::core::allocator<T>>;

}// namespcae ob