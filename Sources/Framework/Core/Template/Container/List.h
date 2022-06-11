//***********************************************************
//! @file
//! @brief		双方向連結リスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <list>
#include <Framework/Core/Template/Allocator/STLAllocator.h>

namespace ob::core {

    //! @brief 双方向連結リスト
    template <class T, class TAlloc = ob::core::allocator<T>>
    using List = std::list<T, TAlloc>;

}// namespcae ob