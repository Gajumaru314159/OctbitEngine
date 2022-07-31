//***********************************************************
//! @file
//! @brief ベクトル
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/Matrix.h>
#include <Framework/Core/Math/Quaternion.h>

#include <Framework/Core/Math/IntVector4.h>



namespace ob::core {

    const Vec2 Vec2::Left(-1.0f, 0.0f);
    const Vec2 Vec2::Right(1.0f, 0.0f);
    const Vec2 Vec2::Down(0.0f, -1.0f);
    const Vec2 Vec2::Up(0.0f, 1.0f);
    const Vec2 Vec2::Zero(0.0f, 0.0f);
    const Vec2 Vec2::One(1.0f, 1.0f);
    const Vec2 Vec2::Minimum(std::numeric_limits<f32>::min());
    const Vec2 Vec2::Maximum(std::numeric_limits<f32>::max());

    const Vec3 Vec3::Left(-1.0f, 0.0f, 0.0f);
    const Vec3 Vec3::Right(1.0f, 0.0f, 0.0f);
    const Vec3 Vec3::Down(0.0f, -1.0f, 0.0f);
    const Vec3 Vec3::Up(0.0f, 1.0f, 0.0f);
    const Vec3 Vec3::Back(0.0f, 0.0f, -1.0f);
    const Vec3 Vec3::Front(0.0f, 0.0f, 1.0f);
    const Vec3 Vec3::Zero(0.0f, 0.0f, 0.0f);
    const Vec3 Vec3::One(1.0f, 1.0f, 1.0f);
    const Vec3 Vec3::Minimum(std::numeric_limits<f32>::min());
    const Vec3 Vec3::Maximum(std::numeric_limits<f32>::max());

    const Vec4 Vec4::Zero(0.0f, 0.0f, 0.0f, 0.0f);
    const Vec4 Vec4::One(1.0f, 1.0f, 1.0f, 1.0f);
    const Vec4 Vec4::Minimum(std::numeric_limits<f32>::min());
    const Vec4 Vec4::Maximum(std::numeric_limits<f32>::max());




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
    Vec3& Vec3::rotate(f32 angle, const Vec3& axis) {
        Quat q(angle, axis);
        *this = axis * (*this);
        return *this;
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