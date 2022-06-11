//***********************************************************
//! @file
//! @brief		単方向リスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <forward_list>
#include <Framework/Core/Template/Allocator/STLAllocator.h>

namespace ob::core {

    //! @brief 単連結リスト
    template <class T, class TAlloc = ob::core::allocator<T>>
    using ForwardList = std::forward_list<T, TAlloc>;

}// namespcae ob