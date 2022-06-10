//***********************************************************
//! @file
//! @brief		トランジション
//! @author		Gajumaru
//***********************************************************
#include "Transition.h"
#include <Framework/Core/Template/include.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  更新
    //@―---------------------------------------------------------------------------
    f32 Transition::get()const noexcept {
        if (time <= 0.0f)return minValue;
        if (length <= time)return minValue;
        if (length < Math::EPSILON)return minValue;

        f32 inRate = 1.0f;
        if (Math::EPSILON < in) {
            inRate = clamp01(time / in);
        }
        f32 outRate = 1.0f;
        if (Math::EPSILON < out) {
            outRate = clamp01((length - time) / out);
        }
        f32 rate = std::min(inRate, outRate);
        return Math::Lerp(minValue, maxValue, rate);
    }

}// namespace ob