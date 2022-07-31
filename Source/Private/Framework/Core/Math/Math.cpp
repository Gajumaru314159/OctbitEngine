//***********************************************************
//! @file
//! @brief f32型算術演算
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Math/Math.h>
#include <limits>

namespace ob::core {
    const f32 Math::PI = 3.1415926535897932384626433832795f;
    const f32 Math::TWO_PI = Math::PI * 2.0f;
    const f32 Math::HALF_PI = Math::PI * 0.5f;
    const f32 Math::INV_PI = 1.0f / Math::PI;
    const f32 Math::INV_TWO_PI = 1.0f / Math::TWO_PI;
    const f32 Math::INV_HALF_PI = 1.0f / Math::HALF_PI;
    const f32 Math::EPSILON = FLT_EPSILON;
    const f32 Math::NAPIER = 2.71828182845904523536f;
    const f32 Math::GOLDEN_RATIO = 1.6180339887498948482045868343656381f;
    const f32 Math::INF = std::numeric_limits<f32>::infinity();
    const f32 Math::NaN = std::numeric_limits<f32>::quiet_NaN();
    const f32 Math::TOLERANCE = 0.00001f;

    //! @brief 値が無効な数字か判定する
    bool Math::IsNaN(f32 value) {
        return isnan(value);
    }

    //! @brief 値が無限か判定する
    bool Math::IsInf(f32 value) {
        return isinf(value);
    }
}