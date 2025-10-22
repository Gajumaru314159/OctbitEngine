//***********************************************************
//! @file
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/Quaternion.h>

#include <Framework/Core/Math/IntVector4.h>



namespace ob::core {

    const Vec2 Vec2::Zero(0.0f, 0.0f);
    const Vec2 Vec2::One(1.0f, 1.0f);
    const Vec2 Vec2::Left(-1.0f, 0.0f);
    const Vec2 Vec2::Right(1.0f, 0.0f);
    const Vec2 Vec2::Down(0.0f, -1.0f);
    const Vec2 Vec2::Up(0.0f, 1.0f);
    const Vec2 Vec2::Minimum(std::numeric_limits<f32>::min());
    const Vec2 Vec2::Maximum(std::numeric_limits<f32>::max());

    const Vec3 Vec3::Zero(0.0f, 0.0f, 0.0f);
    const Vec3 Vec3::One(1.0f, 1.0f, 1.0f);
    const Vec3 Vec3::Left(-1.0f, 0.0f, 0.0f);
    const Vec3 Vec3::Right(1.0f, 0.0f, 0.0f);
    const Vec3 Vec3::Down(0.0f, -1.0f, 0.0f);
    const Vec3 Vec3::Up(0.0f, 1.0f, 0.0f);
    const Vec3 Vec3::Back(0.0f, 0.0f, -1.0f);
    const Vec3 Vec3::Front(0.0f, 0.0f, 1.0f);
    const Vec3 Vec3::Minimum(std::numeric_limits<f32>::min());
    const Vec3 Vec3::Maximum(std::numeric_limits<f32>::max());

    const Vec4 Vec4::Zero(0.0f, 0.0f, 0.0f, 0.0f);
    const Vec4 Vec4::One(1.0f, 1.0f, 1.0f, 1.0f);
    const Vec4 Vec4::Minimum(std::numeric_limits<f32>::min());
    const Vec4 Vec4::Maximum(std::numeric_limits<f32>::max());




    //! @brief          任意軸回転
    //! 
    //! @param angle    回転量
    //! @param axis     回転軸
    Vec3& Vec3::rotate(Vec3 axis, f32 angle) {
        Quat q(axis, angle);
        *this = q * (*this);
        return *this;
    }



    //===============================================================
    //  Vec4
    //===============================================================

    //! @brief  コンストラクタ( IntVec4 初期化)
    Vec4::Vec4(const IntVec4& vec)noexcept {
        x = static_cast<f32>(vec.x);
        y = static_cast<f32>(vec.y);
        z = static_cast<f32>(vec.z);
        w = static_cast<f32>(vec.w);
    }

    //! @brief f32*Vec4演算子 
    Vec4 operator * (f32 f, const Vec4& v) noexcept {
        return v * f;
    }


    //! @brief f32/Vec4演算子 
    Vec4 operator / (f32 f, const Vec4& v) noexcept {
        return Vec4(f) / v;
    }
}