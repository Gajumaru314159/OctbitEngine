//***********************************************************
//! @file
//! @brief		Deque(二重終端キュー)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <deque>
#include "../allocator.h"

namespace ob::core {

    //! @brief デック
    template <class T>
    using deque = std::deque<T, allocator<T>>;

}// namespcae ob