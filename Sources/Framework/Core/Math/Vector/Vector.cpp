﻿//***********************************************************
//! @file
//! @brief ベクトル
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Math/Vector/include.h>
#include <Framework/Core/Math/Matrix.h>
#include <Framework/Core/Math/Quaternion.h>

#include <Framework/Core/Math/Vector/IntVector4.h>



namespace ob {

    const Vec2 Vec2::up(0.0f, 1.0f);
    const Vec2 Vec2::down(0.0f, -1.0f);
    const Vec2 Vec2::left(-1.0f, 0.0f);
    const Vec2 Vec2::right(1.0f, 0.0f);
    const Vec2 Vec2::zero(0.0f, 0.0f);
    const Vec2 Vec2::one(1.0f, 1.0f);

    const Vec3 Vec3::up(0.0f, 1.0f, 0.0f);
    const Vec3 Vec3::down(0.0f, -1.0f, 0.0f);
    const Vec3 Vec3::left(-1.0f, 0.0f, 0.0f);
    const Vec3 Vec3::right(1.0f, 0.0f, 0.0f);
    const Vec3 Vec3::front(0.0f, 0.0f, 1.0f);
    const Vec3 Vec3::back(0.0f, 0.0f, -1.0f);
    const Vec3 Vec3::zero(0.0f, 0.0f, 0.0f);
    const Vec3 Vec3::one(1.0f, 1.0f, 1.0f);

    const Vec4 Vec4::zero(0.0f, 0.0f, 0.0f, 0.0f);
    const Vec4 Vec4::one(1.0f, 1.0f, 1.0f, 1.0f);




    //===============================================================
    //  Vec2
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief f32*Vec2演算子 
    //@―---------------------------------------------------------------------------
    Vec2 operator * (f32 f, const Vec2& v) noexcept {
        return v * f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief f32/Vec2演算子 
    //@―---------------------------------------------------------------------------
    Vec2 operator / (f32 f, const Vec2& v) noexcept {
        return Vec2(f) / v;
    }




    //===============================================================
    //  Vec3
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief f32*Vec3演算子 
    //@―---------------------------------------------------------------------------
    Vec3 operator * (f32 f, const Vec3& v) noexcept {
        return v * f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief f32/Vec3演算子 
    //@―---------------------------------------------------------------------------
    Vec3 operator / (f32 f, const Vec3& v) noexcept {
        return Vec3(f) / v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          任意軸回転
    //! 
    //! @param angle    回転量
    //! @param axis     回転軸
    //@―---------------------------------------------------------------------------
    void Vec3::rotate(f32 angle, const Vec3& axis) {
        Quat q(angle, axis);
        *this = axis * (*this);
    }



    //===============================================================
    //  Vec4
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ( IntVec4 初期化)
    //@―---------------------------------------------------------------------------
    Vec4::Vec4(const IntVec4& vec)noexcept {
        x = static_cast<f32>(vec.x);
        y = static_cast<f32>(vec.y);
        z = static_cast<f32>(vec.z);
        w = static_cast<f32>(vec.w);
    }

    //@―---------------------------------------------------------------------------
    //! @brief f32*Vec4演算子 
    //@―---------------------------------------------------------------------------
    Vec4 operator * (f32 f, const Vec4& v) noexcept {
        return v * f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief f32/Vec4演算子 
    //@―---------------------------------------------------------------------------
    Vec4 operator / (f32 f, const Vec4& v) noexcept {
        return Vec4(f) / v;
    }
}