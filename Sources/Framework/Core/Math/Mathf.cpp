//***********************************************************
//! @file
//! @brief f32型算術演算
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Math/Mathf.h>
#include <limits>

namespace ob::core {
    const f32 Mathf::PI = 3.1415926535897932384626433832795f;
    const f32 Mathf::TWO_PI = Mathf::PI * 2.0f;
    const f32 Mathf::HALF_PI = Mathf::PI * 0.5f;
    const f32 Mathf::INV_PI = 1.0f / Mathf::PI;
    const f32 Mathf::INV_TWO_PI = 1.0f / Mathf::TWO_PI;
    const f32 Mathf::INV_HALF_PI = 1.0f / Mathf::HALF_PI;
    const f32 Mathf::EPSILON = FLT_EPSILON;
    const f32 Mathf::NAPIER = 2.71828182845904523536f;
    const f32 Mathf::GOLDEN_RATIO = 1.6180339887498948482045868343656381f;
    const f32 Mathf::INF = std::numeric_limits<f32>::infinity();
    const f32 Mathf::NaN = std::numeric_limits<f32>::quiet_NaN();
    const f32 Mathf::TOLERANCE = 0.00001f;

    //! @brief 値が無効な数字か判定する
    bool Mathf::IsNaN(f32 value) {
        return isnan(value);
    }

    //! @brief 値が無限か判定する
    bool Mathf::IsInf(f32 value) {
        return isinf(value);
    }
}