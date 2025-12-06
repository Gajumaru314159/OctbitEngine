//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <atomic>

namespace ob::core {

    //! @brief  アトミック型
    template<typename T>
    using Atomic = std::atomic<T>;

}