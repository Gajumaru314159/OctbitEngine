//***********************************************************
//! @file
//! @brief		Futureパターン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <future>

namespace ob::core {

    using std::future_errc;
    using std::launch;
    using std::future_status;
    using std::is_error_code_enum;
    using std::make_error_code;
    using std::future_category;
    using std::future_error;
    using std::promise;
    using std::future;
    using std::shared_future;
    using std::packaged_task;
    using std::async;

}// namespcae ob