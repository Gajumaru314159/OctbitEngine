﻿//***********************************************************
//! @file
//! @brief		スフィア構造体
//! @author		Gajumaru
//***********************************************************
#include <Core/Math/Geometory/Sphere.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子(合成)
    //@―---------------------------------------------------------------------------
    Sphere Sphere::operator+(const Vec3& point)const {
        return Sphere(*this) += point;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子(合成)
    //@―---------------------------------------------------------------------------
    Sphere& Sphere::operator+=(const Vec3& point) {
        Vec3 dir;
        f32 len;
        (point - center).ToDirectionAndLength(dir, len);
        f32 diff = Mathf::Max(len - radius, 0.0f) * 0.5f;
        center += dir * diff;
        radius += diff;
        return *this;
    }

}// namespace ob