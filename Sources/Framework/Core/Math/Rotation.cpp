﻿//***********************************************************
//! @file
//! @brief		オイラー角回転構造体
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/Rotation.h>
#include <Framework/Core/Math/Vector/Vector3.h>
#include <Framework/Core/Math/Quaternion.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(Vec3指定)
    //@―---------------------------------------------------------------------------
    Rot::Rot(const Vec3& vec)noexcept {
        set(vec);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(Quat指定)
    //@―---------------------------------------------------------------------------
    Rot::Rot(const Quat& quat) {
        *this = quat.getRot();
    }

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(Vec3指定)
    //@―---------------------------------------------------------------------------
    void Rot::set(const Vec3& vec)noexcept {
        set(vec.x, vec.y, vec.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Quatに変換
    //@―---------------------------------------------------------------------------
    Vec3 Rot::toVec3()const {
        return Vec3(roll, pitch, yaw);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Quatに変換
    //@―---------------------------------------------------------------------------
    inline Quat Rot::toQuat()const {
        return Quat(roll, pitch, yaw);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  正面ベクトルを取得
    //@―---------------------------------------------------------------------------
    inline Vec3 Rot::front()const {
        return toQuat().getFront();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  背面ベクトルを取得
    //@―---------------------------------------------------------------------------
    inline Vec3 Rot::back()const {
        return toQuat().getBack();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  上ベクトルを取得
    //@―---------------------------------------------------------------------------
    inline Vec3 Rot::up()const {
        return toQuat().getUp();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  下ベクトルを取得
    //@―---------------------------------------------------------------------------
    inline Vec3 Rot::down()const {
        return toQuat().getDown();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  左ベクトルを取得
    //@―---------------------------------------------------------------------------
    inline Vec3 Rot::left()const {
        return toQuat().getLeft();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  右ベクトルを取得
    //@―---------------------------------------------------------------------------
    inline Vec3 Rot::right()const {
        return toQuat().getRight();
    }



}// namespace ob