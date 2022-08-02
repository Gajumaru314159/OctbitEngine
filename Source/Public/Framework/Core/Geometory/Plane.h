﻿//***********************************************************
//! @file
//! @brief		平面
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector3.h>
#include <Framework/Core/Geometory/Triangle.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief 平面
    //@―---------------------------------------------------------------------------
    struct OB_API Plane {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Plane() = default;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(平面の方程式の係数で初期化)
        //@―---------------------------------------------------------------------------
        Plane(f32 x, f32 y, f32 z, f32 w) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(法線と原点からの距離で初期化)
        //@―---------------------------------------------------------------------------
        Plane(const Vec3& normal, f32 distance);


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(平面上の1点と法線で初期化)
        //@―---------------------------------------------------------------------------
        Plane(const Vec3& base, const Vec3& normal);


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(3点で初期化)
        //@―---------------------------------------------------------------------------
        Plane(const Vec3& pos1, const Vec3& pos2, const Vec3& pos3);


        //===============================================================
        //  変換
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 面を反転
        //@―---------------------------------------------------------------------------
        Plane& flip() noexcept;


    public:

        Vec3    normal;         //!< 面の法線
        f32     distance=0;     //!< 原点と面の距離

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(平面の方程式の係数で初期化)
    //@―---------------------------------------------------------------------------
    inline Plane::Plane(f32 x, f32 y, f32 z, f32 w) noexcept {
        normal.set(x, y, z);
        distance = w * normal.length();
        normal.normalize();
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(法線と原点からの距離で初期化)
    //@―---------------------------------------------------------------------------
    inline Plane::Plane(const Vec3& normal, f32 distance) {
        this->normal = normal;
        this->distance = distance;
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(平面上の1点と法線で初期化)
    //@―---------------------------------------------------------------------------
    inline Plane::Plane(const Vec3& base, const Vec3& n) {
        normal = n;
        normal.normalize();
        distance = Vec3::Dot(normal, base);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(3点で初期化)
    //@―---------------------------------------------------------------------------
    inline Plane::Plane(const Vec3& pos1, const Vec3& pos2, const Vec3& pos3) {
        normal=Triangle(pos1, pos2, pos3).normal();
        distance = Vec3::Dot(normal, pos1);
    }


    //===============================================================
    //  変換
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 面を反転
    //@―---------------------------------------------------------------------------
    inline Plane& Plane::flip() noexcept {
        normal *= -1.0f;
        return *this;
    }

    //! @endcond
}// namespcae ob