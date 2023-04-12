﻿//***********************************************************
//! @file
//! @brief		整数ベクトル
//! @author		Gajumaru
//***********************************************************

#include <Framework/Core/Math/IntVector2.h>
#include <Framework/Core/Math/IntVector3.h>
#include <Framework/Core/Math/IntVector4.h>
#include <Framework/Core/Math/Vector2.h>
#include <Framework/Core/Math/Vector3.h>
#include <Framework/Core/Math/Vector4.h>
namespace ob::core {

    const IntVec2 IntVec2::Left(-1, 0);
    const IntVec2 IntVec2::Right(1, 0);
    const IntVec2 IntVec2::Down(0, -1);
    const IntVec2 IntVec2::Up(0, 1);
    const IntVec2 IntVec2::Zero(0, 0);
    const IntVec2 IntVec2::One(1, 1);
    const IntVec2 IntVec2::Minimum(std::numeric_limits<s32>::min());
    const IntVec2 IntVec2::Maximum(std::numeric_limits<s32>::max());

    const IntVec3 IntVec3::Left(-1, 0, 0);
    const IntVec3 IntVec3::Right(1, 0, 0);
    const IntVec3 IntVec3::Down(0, -1, 0);
    const IntVec3 IntVec3::Up(0, 1, 0);
    const IntVec3 IntVec3::Back(0, 0, -1);
    const IntVec3 IntVec3::Front(0, 0, 1);
    const IntVec3 IntVec3::Zero(0, 0, 0);
    const IntVec3 IntVec3::One(1, 1, 1);
    const IntVec3 IntVec3::Minimum(std::numeric_limits<s32>::min());
    const IntVec3 IntVec3::Maximum(std::numeric_limits<s32>::max());

    const IntVec4 IntVec4::Zero(0, 0, 0, 0);
    const IntVec4 IntVec4::One(1, 1, 1, 1);
    const IntVec4 IntVec4::Minimum(std::numeric_limits<s32>::min());
    const IntVec4 IntVec4::Maximum(std::numeric_limits<s32>::max());


    //===============================================================
    //  IntVec2
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! コンストラクタ( Vec2 初期化)
    //@―---------------------------------------------------------------------------
    IntVec2::IntVec2(const Vec2& another)noexcept {
        x = static_cast<s32>(another.x);
        y = static_cast<s32>(another.y);
    }


    //@―---------------------------------------------------------------------------
    //! @brief s32*IntVec2演算子
    //@―---------------------------------------------------------------------------
    IntVec2 operator * (s32 f, const IntVec2& v)noexcept {
        return v * f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief s32/IntVec2演算子
    //@―---------------------------------------------------------------------------
    IntVec2 operator / (s32 f, const IntVec2& v)noexcept {
        return IntVec2(f, f) / v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief Vec4 で取得
    //@―---------------------------------------------------------------------------
    Vec2 IntVec2::toVec2() const noexcept {
        return Vec2(1.0f * x, 1.0f * y);
    }





    //===============================================================
    //  IntVec3
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! コンストラクタ( Vec3 初期化)
    //@―---------------------------------------------------------------------------
    IntVec3::IntVec3(const Vec3& another)noexcept {
        x = static_cast<s32>(another.x);
        y = static_cast<s32>(another.y);
        z = static_cast<s32>(another.z);
    }

    //@―---------------------------------------------------------------------------
    //! @brief s32*IntVec3演算子
    //@―---------------------------------------------------------------------------
    IntVec3 operator * (s32 f, const IntVec3& v)noexcept {
        return v * f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief s32/IntVec3演算子
    //@―---------------------------------------------------------------------------
    IntVec3 operator / (s32 f, const IntVec3& v)noexcept {
        return IntVec3(f, f, f) / v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief Vec3 で取得
    //@―---------------------------------------------------------------------------
    Vec3 IntVec3::toVec3() const noexcept {
        return Vec3(1.0f * x, 1.0f * y, 1.0f * z);
    }




    //===============================================================
    //  IntVec4
    //===============================================================

    //@―---------------------------------------------------------------------------
    // コンストラクタ( Vec4 初期化)
    //@―---------------------------------------------------------------------------
    IntVec4::IntVec4(const Vec4& vec)noexcept {
        x = static_cast<s32>(vec.x);
        y = static_cast<s32>(vec.y);
        z = static_cast<s32>(vec.z);
        w = static_cast<s32>(vec.w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief s32*IntVec4演算子 
    //@―---------------------------------------------------------------------------
    IntVec4 operator * (s32 f, const IntVec4& v)noexcept {
        return v * f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief s32/IntVec4演算子 
    //@―---------------------------------------------------------------------------
    IntVec4 operator / (s32 f, const IntVec4& v)noexcept {
        return IntVec4(f, f, f, f) / v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief Vec4 で取得
    //@―---------------------------------------------------------------------------
    Vec4 IntVec4::toVec4() const noexcept {
        return Vec4(1.0f * x, 1.0f * y, 1.0f * z, 1.0f * w);
    }

}