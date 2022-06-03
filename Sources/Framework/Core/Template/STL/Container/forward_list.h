//***********************************************************
//! @file
//! @brief		単方向リスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <forward_list>
#include "../allocator.h"

namespace ob::core {

    //! @brief 単連結リスト
    template <class T>
    using forward_list = std::forward_list<T, allocator<T>>;

}// namespcae ob