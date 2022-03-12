//***********************************************************
//! @file
//! @brief		メモリー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <memory>

namespace ob::foundation {

    using std::uninitialized_default_construct;
    using std::uninitialized_default_construct_n;
    using std::uninitialized_value_construct;
    using std::uninitialized_value_construct_n;
    using std::uninitialized_copy;
    using std::uninitialized_copy_n;
    using std::uninitialized_move;
    using std::uninitialized_move_n;
    using std::uninitialized_fill;
    using std::uninitialized_fill_n;
    using std::destroy_at;
    using std::destroy;
    using std::destroy_n;

    using std::pointer_traits;
    using std::addressof;
    using std::align;

}// namespcae ob