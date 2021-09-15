//***********************************************************
//! @file
//! @brief		時間ユーティリティ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <chrono>

namespace ob
{
    namespace chrono
    {

        using std::chrono::duration;
        using std::chrono::time_point;
        using std::chrono::treat_as_floating_point;
        using std::chrono::duration_values;

        using std::chrono::nanoseconds;
        using std::chrono::microseconds;
        using std::chrono::milliseconds;
        using std::chrono::seconds;
        using std::chrono::minutes;
        using std::chrono::hours;


        using std::chrono::system_clock;
        using std::chrono::steady_clock;
        using std::chrono::high_resolution_clock;

    }
}// namespcae ob