//***********************************************************
//! @file
//! @brief		Variant
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <variant>

namespace ob::core {

    using std::variant;
    using std::variant_size;
    using std::variant_alternative;
    using std::variant_npos;
    using std::holds_alternative;
    using std::visit;
    using std::monostate;
    using std::bad_variant_access;

}// namespcae ob