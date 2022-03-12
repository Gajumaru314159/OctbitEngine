//***********************************************************
//! @file
//! @brief		矩形
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometory/Rect.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(点群指定)
    //! 
    //! @details    指定された点群をすべて含むボックスを構築する
    //@―---------------------------------------------------------------------------
    Rect::Rect(const gsl::span<Vec2>& points)noexcept {
        if (points.size() <= 1) {
            reset();
            return;
        }
        Vec2 min = points[0];
        Vec2 max = points[0];
        for (auto& point : points) {
            min = Vec2::Min(min, point);
            max = Vec2::Max(max, point);
        }
        left = min.x;
        top = min.y;
        right = max.x;
        bottom = max.y;
    }

}// namespcae ob