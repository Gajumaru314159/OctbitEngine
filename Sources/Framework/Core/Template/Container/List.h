//***********************************************************
//! @file
//! @brief		双方向連結リスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <list>
#include <Framework/Core/Template/STLAllocator.h>

namespace ob::core {

    //! @brief 双方向連結リスト
    template <class T>
    using list = std::list<T, ob::core::allocator<T>>;

}// namespcae ob