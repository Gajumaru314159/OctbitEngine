//***********************************************************
//! @file
//! @brief		アトミック型
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <atomic>

namespace ob
{

    using std::memory_order;
    using std::kill_dependency;

    using std::atomic;

    using std::atomic_is_lock_free;
    using std::atomic_init;
    using std::atomic_store;
    using std::atomic_store_explicit;
    using std::atomic_load;
    using std::atomic_load_explicit;
    using std::atomic_exchange;
    using std::atomic_exchange_explicit;
    using std::atomic_compare_exchange_weak;
    using std::atomic_compare_exchange_strong;
    using std::atomic_compare_exchange_weak_explicit;
    using std::atomic_compare_exchange_strong_explicit;

    using std::atomic_fetch_add;
    using std::atomic_fetch_add_explicit;
    using std::atomic_fetch_sub;
    using std::atomic_fetch_sub_explicit;
    using std::atomic_fetch_and;
    using std::atomic_fetch_and_explicit;
    using std::atomic_fetch_or;
    using std::atomic_fetch_or_explicit;
    using std::atomic_fetch_xor;
    using std::atomic_fetch_xor_explicit;

    using std::atomic_flag;
    using std::atomic_flag_test_and_set;
    using std::atomic_flag_test_and_set_explicit;
    using std::atomic_flag_clear;
    using std::atomic_flag_clear_explicit;

    using std::atomic_thread_fence;
    using std::atomic_signal_fence;

}// namespcae ob