//***********************************************************
//! @file
//! @brief		関数オブジェクト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <functional>

namespace ob
{

    using std::function;
    using std::bad_function_call;

    using std::invoke;

    using std::bind;
    using std::is_bind_expression;
    using std::is_placeholder;
    using std::mem_fn;

    using std::reference_wrapper;
    using std::ref;
    using std::cref;

    using std::hash;

    namespace placeholders
    {
        using namespace std::placeholders;
    }

    // 関数オブジェクト
    using std::plus;
    using std::minus;
    using std::multiplies;
    using std::divides;
    using std::modulus;
    using std::negate;

    using std::equal_to;
    using std::not_equal_to;
    using std::less;
    using std::less_equal;
    using std::greater;
    using std::greater_equal;

    using std::logical_and;
    using std::logical_or;
    using std::logical_not;

    using std::bit_and;
    using std::bit_or;
    using std::bit_xor;
    using std::bit_not;

}// namespcae ob