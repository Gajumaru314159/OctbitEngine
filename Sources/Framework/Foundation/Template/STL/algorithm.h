//***********************************************************
//! @file
//! @brief		テンプレート・アルゴリズム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <algorithm>

namespace ob::foundation {

    using std::all_of;
    using std::any_of;
    using std::none_of;
    using std::for_each;
    using std::for_each_n;
    using std::find;
    using std::find_if;
    using std::find_if_not;
    using std::find_end;
    using std::find_first_of;
    using std::adjacent_find;
    using std::count;
    using std::count_if;
    using std::mismatch;
    using std::equal;
    using std::search;
    using std::search_n;

    using std::copy;
    using std::copy_n;
    using std::copy_if;
    using std::copy_backward;
    using std::move;
    using std::move_backward;
    using std::swap_ranges;
    using std::iter_swap;
    using std::transform;
    using std::replace;
    using std::replace_if;
    using std::replace_copy;
    using std::replace_copy_if;
    using std::fill;
    using std::fill_n;
    using std::generate;
    using std::generate_n;
    using std::remove;
    using std::remove_if;
    using std::remove_copy;
    using std::remove_copy_if;
    using std::unique;
    using std::unique_copy;
    using std::reverse;
    using std::reverse_copy;
    using std::rotate;
    using std::rotate_copy;
    using std::sample;
    using std::shuffle;
    using std::is_partitioned;
    using std::partition;
    using std::stable_partition;
    using std::partition_copy;
    using std::partition_point;

}// namespcae ob