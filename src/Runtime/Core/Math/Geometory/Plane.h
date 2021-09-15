﻿//***********************************************************
//! @file
//! @brief		平面構造体
//! @author		Gajumaru
//***********************************************************
#pragma once
#include<Core/Math/Vector/include.h>

namespace ob {

    //-----------------------------------------------------------------------------
    //! @brief 平面構造体
    //-----------------------------------------------------------------------------
    struct Plane {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        Plane() noexcept;                                                       // デフォルトコンストラクタ
        Plane(EForceInit) noexcept;                                             // コンストラクタ(XZ平面で初期化)
        Plane(f32 x, f32 y, f32 z, f32 w) noexcept;                             // コンストラクタ(平面の方程式の係数で初期化)
        Plane(const Vec3& normal, f32 w);                                       // コンストラクタ(法線と原点からの距離で初期化)
        Plane(const Vec3& base, const Vec3& normal);                            // コンストラクタ(平面上の1点と法線で初期化)
        Plane(const Vec3& pos1, const Vec3& pos2, const Vec3& pos3);            // コンストラクタ(3点で初期化)

        //===============================================================
        //  変換
        //===============================================================
        void Flip() noexcept;                                                   // 面の反転


    public:

        Vec3    normal;         //!< 面の法線
        f32     distance;       //!< 原点と面の距離

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------------------------


}// namespcae ob