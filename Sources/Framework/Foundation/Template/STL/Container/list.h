//***********************************************************
//! @file
//! @brief		双方向連結リスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <list>
#include "../allocator.h"

namespace ob {

    template <class T>
    using list = std::list<T, allocator<T>>;

}// namespcae ob