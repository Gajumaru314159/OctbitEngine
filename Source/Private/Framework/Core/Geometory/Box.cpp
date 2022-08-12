//***********************************************************
//! @file
//! @brief		ボックス
//! @author		Gajumaru
//***********************************************************

#include <Framework/Core/Geometory/Box.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(点群指定)
    //! 
    //! @details    指定された点群をすべて含むボックスを構築する。
    //!             要素数が1以下の時は reset() が呼び出される。
    //@―---------------------------------------------------------------------------
    Box::Box(Span<Vec3> points)noexcept {
        if (points.size() <= 1) {
            center = Vec3::Zero;
            size = Vec3::Zero;
        } else {
            Vec3 min = points[0];
            Vec3 max = points[0];
            for (auto& point : points) {
                min = Vec3::Min(min, point);
                max = Vec3::Max(max, point);
            }
            center = (min + max) * 0.5f;
            size = max - min;
        }
    }

}// namespace ob