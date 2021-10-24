//***********************************************************
//! @file
//! @brief		単方向リスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Foundation/Template/allocator.h>
#include <forward_list>

namespace ob
{

    template <class T>
    using forward_list = std::forward_list<T, allocator<T>>;

}// namespcae ob