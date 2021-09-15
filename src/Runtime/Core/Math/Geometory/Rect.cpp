//***********************************************************
//! @file
//! @brief		矩形構造体
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Math/Geometory/Rect.h>

namespace ob {

    //-----------------------------------------------------------------------------
    //! @brief      コンストラクタ(点群指定)
    //! 
    //! @details    指定された点群をすべて含むボックスを構築する
    //-----------------------------------------------------------------------------
    Rect::Rect(const Vec2* points, size_t count) {
        if (count == 0) {
            Reset();
            return;
        }
        Vec2 min = points[0];
        Vec2 max = points[0];
        for (s32 i = 1; i < count; i++) {
            min = Vec2::Min(min, points[i]);
            max = Vec2::Max(min, points[i]);
        }
        center = (min + max) * 0.5f;
        size = max - min;
    }

}// namespcae ob