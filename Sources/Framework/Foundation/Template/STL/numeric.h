//***********************************************************
//! @file
//! @brief		数値シーケンス処理
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <numeric>

namespace ob {

    using std::accumulate;
    using std::reduce;
    using std::transform_reduce;

    using std::inner_product;

    using std::partial_sum;
    using std::inclusive_scan;
    using std::exclusive_scan;
    using std::transform_inclusive_scan;
    using std::transform_exclusive_scan;

    using std::adjacent_difference;
    using std::iota;

    using std::gcd;
    using std::lcm;

}// namespcae ob