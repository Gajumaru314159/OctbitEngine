//***********************************************************
//! @file
//! @brief		ミューテックス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <mutex>

namespace ob {

    using std::mutex;
    using std::recursive_mutex;
    using std::timed_mutex;
    using std::recursive_timed_mutex;

    using std::defer_lock;
    using std::try_to_lock;
    using std::adopt_lock;
    using std::lock_guard;
    using std::scoped_lock;
    using std::unique_lock;
    using std::try_lock;
    using std::lock;

    using std::once_flag;
    using std::call_once;

}// namespcae ob