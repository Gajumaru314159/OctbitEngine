//***********************************************************
//! @file
//! @brief		ユーティリティ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <utility>

namespace ob::foundation {

    using std::swap;
    using std::exchange;

    using std::forward;
    using std::move;
    using std::move_if_noexcept;

    using std::as_const;

    using std::declval;

    using std::pair;
    using std::make_pair;
    using std::in_place_t;
    using std::in_place;
    using std::in_place_type_t;
    using std::in_place_type;
    using std::in_place_index_t;
    using std::in_place_index;

    using std::integer_sequence;
    using std::make_integer_sequence;
    using std::index_sequence;
    using std::make_index_sequence;
    using std::index_sequence_for;

}// namespcae ob