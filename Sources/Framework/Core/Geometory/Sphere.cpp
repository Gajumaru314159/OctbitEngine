﻿//***********************************************************
//! @file
//! @brief		スフィア
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometory/Sphere.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子(合成)
    //@―---------------------------------------------------------------------------
    Sphere Sphere::operator+(const Vec3& point)const noexcept {
        return Sphere(*this) += point;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子(合成)
    //@―---------------------------------------------------------------------------
    Sphere& Sphere::operator+=(const Vec3& point)noexcept {
        Vec3 dir;
        f32 len;
        (point - center).toDirectionAndLength(dir, len);
        f32 diff = Mathf::Max(len - radius, 0.0f) * 0.5f;
        center += dir * diff;
        radius += diff;
        return *this;
    }

}// namespace ob