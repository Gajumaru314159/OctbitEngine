﻿//***********************************************************
//! @file
//! @brief		可変長配列
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <vector>
#include "../allocator.h"

namespace ob {

    template <class T>
    using vector = std::vector<T, allocator<T>>;

}// namespcae ob