//***********************************************************
//! @file
//! @brief		トランジション
//! @author		Gajumaru
//***********************************************************
#include "Transition.h"

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  更新
    //@―---------------------------------------------------------------------------
    f32 Transition::get()const noexcept {
        if (time <= 0.0f)return minValue;
        if (length <= time)return minValue;
        if (length < Mathf::EPSILON)return minValue;

        f32 inRate = 1.0f;
        if (Mathf::EPSILON < in) {
            inRate = get_clamp01(time / in);
        }
        f32 outRate = 1.0f;
        if (Mathf::EPSILON < out) {
            outRate = get_clamp01((length-time)/out);
        }
        f32 rate = get_min(inRate,outRate);
        return Mathf::Lerp(minValue, maxValue, rate);
    }

}// namespace ob