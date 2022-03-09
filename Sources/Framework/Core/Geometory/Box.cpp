//***********************************************************
//! @file
//! @brief		ボックス構造体
//! @author		Gajumaru
//***********************************************************

#include <Framework/Core/Geometory/Box.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(点群指定)
    //! 
    //! @details    指定された点群をすべて含むボックスを構築する。
    //!             要素数が1以下の時は Reset() が呼び出される。
    //@―---------------------------------------------------------------------------
    Box::Box(const gsl::span<Vec3>& points)noexcept {
        Set(points);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      点群から Box を設定
    //! 
    //! @detail     点群を含む最小の Box に設定する。
    //!             要素数が1以下の時は Reset() が呼び出される。
    //@―---------------------------------------------------------------------------
    void Box::Set(const gsl::span<Vec3>& points)noexcept {
        if (points.size()<=1) {
            Reset();
            return;
        }
        Vec3 min = points[0];
        Vec3 max = points[0];
        for (auto& point:points) {
            min = Vec3::Min(min, point);
            max = Vec3::Max(max, point);
        }
        center = (min + max) * 0.5f;
        size = max - min;
    }

}// namespace ob