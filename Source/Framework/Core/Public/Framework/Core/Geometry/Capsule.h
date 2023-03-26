﻿//***********************************************************
//! @file
//! @brief		カプセル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector3.h>
#include <Framework/Core/Math/Quaternion.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  カプセル構造体
    //! 
    //! @note   radius が負の数の時は未定義動作です。
    //@―---------------------------------------------------------------------------
    struct OB_API Capsule {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        inline Capsule() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ(2点指定)
        //! 
        //! @param pos1     始点
        //! @param pos2     終点
        //! @param radius   半径
        //@―---------------------------------------------------------------------------
        constexpr Capsule(const Vec3 pos1, const Vec3 pos2, f32 radius) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ(中心/Quat指定)
        //! 
        //! @param center   中心点
        //! @param radius   半径
        //! @param height   カプセルの高さ(半径を含まない)
        //! @param quat     カプセルの回転(無回転でY-up方向)
        //@―---------------------------------------------------------------------------
        Capsule(const Vec3& center, f32 height, f32 radius, const Quat& quat) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief           コンストラクタ(中心/方向指定)
        //!                  
        //! @param center    中心点
        //! @param radius    半径
        //! @param height    カプセルの高さ(半径を含まない)
        //! @param direction カプセルの上方向
        //@―---------------------------------------------------------------------------
        Capsule(const Vec3& center, f32 height, f32 radius, const Vec3& direction);


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief           高さを取得
        //@―---------------------------------------------------------------------------
        f32 height()const;


        //@―---------------------------------------------------------------------------
        //! @brief           半径を含まない高さを取得
        //@―---------------------------------------------------------------------------
        f32 minHeight()const;


        //@―---------------------------------------------------------------------------
        //! @brief           pos1->pos2の方向ベクトルを取得
        //@―---------------------------------------------------------------------------
        Vec3 direction()const;


        //@―---------------------------------------------------------------------------
        //! @brief           体積を取得
        //@―---------------------------------------------------------------------------
        f32 volume()const noexcept;

    public:

        Vec3    pos1;       //!< 始点
        Vec3    pos2;       //!< 終点
        f32     radius;     //!< 半径

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond


    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタ(初期化なし)
    //@―---------------------------------------------------------------------------
    inline Capsule::Capsule() noexcept
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタ(2点指定)
    //! 
    //! @param pos1     始点
    //! @param pos2     終点
    //! @param radius   半径
    //@―---------------------------------------------------------------------------
    constexpr Capsule::Capsule(const Vec3 pos1, const Vec3 pos2, f32 radius) noexcept
        : pos1(pos1),pos2(pos2),radius(radius)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタ(中心/Quat指定)
    //! 
    //! @param center   中心点
    //! @param radius   半径
    //! @param height   カプセルの高さ(半径を含まない)
    //! @param quat     カプセルの回転(無回転でY-up方向)
    //@―---------------------------------------------------------------------------
    inline Capsule::Capsule(const Vec3& center, f32 height, f32 radius, const Quat& quat)noexcept {
        this->radius = radius;
        const Vec3 half = quat.up() * (height * 0.5f);
        pos1 = center + half;
        pos2 = center - half;
    }


    //@―---------------------------------------------------------------------------
    //! @brief           コンストラクタ(中心/方向指定)
    //!                  
    //! @param center    中心点
    //! @param radius    半径
    //! @param height    カプセルの高さ(半径を含まない)
    //! @param direction カプセルの上方向
    //@―---------------------------------------------------------------------------
    inline Capsule::Capsule(const Vec3& center, f32 height, f32 radius, const Vec3& direction) {
        this->radius = radius;
        const Vec3 half = direction.unitVec() * (height * 0.5f);
        pos1 = center + half;
        pos2 = center - half;
    }


    //@―---------------------------------------------------------------------------
    //! @brief           高さを取得
    //@―---------------------------------------------------------------------------
    inline f32 Capsule::height()const {
        return Vec3::Dist(pos1, pos2) + radius;
    }


    //@―---------------------------------------------------------------------------
    //! @brief           半径を含まない高さを取得
    //@―---------------------------------------------------------------------------
    inline f32 Capsule::minHeight()const {
        return Vec3::Dist(pos1, pos2);
    }


    //@―---------------------------------------------------------------------------
    //! @brief           pos1->pos2の方向ベクトルを取得
    //@―---------------------------------------------------------------------------
    inline Vec3 Capsule::direction()const {
        return (pos2 - pos1).unitVec();
    }


    //@―---------------------------------------------------------------------------
    //! @brief           体積を取得
    //@―---------------------------------------------------------------------------
    inline f32 Capsule::volume()const noexcept {
        return
            radius * radius * Math::PI * minHeight() +
            radius * radius * radius * Math::PI * 4.0f / 3.0f;
    }

    //! @endcond
}// namespcae ob