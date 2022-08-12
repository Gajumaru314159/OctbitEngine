﻿//***********************************************************
//! @file
//! @brief		スフィア
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector3.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  スフィア
    //! 
    //! @note   radius が負の数の時は未定義動作です。
    //@―---------------------------------------------------------------------------
    struct OB_API Sphere {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Sphere() noexcept = default;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(中心/サイズ指定)
        //@―---------------------------------------------------------------------------
        constexpr Sphere(const Vec3& center, f32 radius) noexcept;


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator==(const Sphere& other)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator!=(const Sphere& other)const noexcept;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      補間
        //! 
        //! @param a    開始
        //! @param b    終了
        //! @param t    補間係数
        //! @return     t=0のときa、t=1の時bを返す。
        //@―---------------------------------------------------------------------------
        static constexpr Sphere Lerp(const Sphere& a, const Sphere& b, f32 t)noexcept;


    public:

        Vec3    center;     //!< 中心座標
        f32     radius;     //!< 半径

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(中心/サイズ指定)
    //@―---------------------------------------------------------------------------
    constexpr Sphere::Sphere(const Vec3& center, f32 radius) noexcept
        : center(center),radius(radius)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool Sphere::operator==(const Sphere& other)const noexcept {
        return center == other.center && Math::IsNearEquals(radius, other.radius);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool Sphere::operator!=(const Sphere& other)const noexcept {
        return  !(operator==(other));
    }


    //@―---------------------------------------------------------------------------
    //! @brief      補間
    //! 
    //! @param a    開始
    //! @param b    終了
    //! @param t    補間係数
    //! @return     t=0のときa、t=1の時bを返す。
    //@―---------------------------------------------------------------------------
    constexpr Sphere Sphere::Lerp(const Sphere& a, const Sphere& b, f32 t)noexcept {
        return Sphere(Vec3::Lerp(a.center, b.center, t), Math::Lerp(a.radius, b.radius, t));
    }

    //! @endcond
}// namespcae ob