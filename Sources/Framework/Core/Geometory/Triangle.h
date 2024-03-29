﻿//***********************************************************
//! @file
//! @brief		三角形
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector/include.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  三角形
    //@―---------------------------------------------------------------------------
    class OB_API Triangle {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Triangle() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(Vec3)
        //@―---------------------------------------------------------------------------
        Triangle(Vec3 p0, Vec3 p1, Vec3 p2);


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(Vec2)
        //@―---------------------------------------------------------------------------
        Triangle(Vec2 p0, Vec2 p1, Vec2 p2);


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          等価演算子
        //@―---------------------------------------------------------------------------
        bool operator==(const Triangle& another)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          否等価演算子
        //@―---------------------------------------------------------------------------
        bool operator!=(const Triangle& another)const noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  要点を設定
        //@―---------------------------------------------------------------------------
        void set(Vec2 p0, Vec2 p1, Vec2 p2);


        //@―---------------------------------------------------------------------------
        //! @brief  要点を設定
        //@―---------------------------------------------------------------------------
        void set(Vec3 p0, Vec3 p1, Vec3 p2);


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  法線を取得
        //! 
        //! @details 領域がないときゼロベクトルを返す
        //@―---------------------------------------------------------------------------
        Vec3 normal()const;


        //@―---------------------------------------------------------------------------
        //! @brief  重心を計算
        //@―---------------------------------------------------------------------------
        Vec3 centerOfGravity()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  内心を計算
        //@―---------------------------------------------------------------------------
        Vec3 innerGravity()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  外心を計算
        //@―---------------------------------------------------------------------------
        Vec3 circumcenter()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  面積を計算
        //@―---------------------------------------------------------------------------
        f32 area()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  外周を計算
        //@―---------------------------------------------------------------------------
        f32 perimeter()const noexcept;


        //===============================================================
        //  変換
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  面を反転
        //! 
        //! @details p1 と p2 をスワップします。
        //@―---------------------------------------------------------------------------
        Triangle& flip()noexcept;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      補間
        //! 
        //! @param a    開始
        //! @param b    終了
        //! @param t    補間係数
        //! @return     t=0のときa、t=1の時bを返す。
        //@―---------------------------------------------------------------------------
        static Triangle Lerp(const Triangle& a, const Triangle& b, f32 t)noexcept;


    public:

        Vec3 p0;    //!< 頂点0
        Vec3 p1;    //!< 頂点1
        Vec3 p2;    //!< 頂点2

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(Vec3)
    //@―---------------------------------------------------------------------------
    inline Triangle::Triangle(Vec3 p0, Vec3 p1, Vec3 p2) {
        set(p0, p1, p2);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(Vec2)
    //@―---------------------------------------------------------------------------
    inline Triangle::Triangle(Vec2 p0, Vec2 p1, Vec2 p2) {
        set(p0, p1, p2);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Triangle::operator==(const Triangle& another)const noexcept {
        return
            p0 == another.p0 &&
            p1 == another.p1 &&
            p2 == another.p2;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          否等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Triangle::operator!=(const Triangle& another)const noexcept {
        return !(*this == another);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  要点を設定
    //@―---------------------------------------------------------------------------
    inline void Triangle::set(Vec2 p0, Vec2 p1, Vec2 p2) {
        p0.set(p0.x, p0.y);
        p1.set(p1.x, p1.y);
        p2.set(p2.x, p2.y);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  要点を設定
    //@―---------------------------------------------------------------------------
    inline void Triangle::set(Vec3 p0, Vec3 p1, Vec3 p2) {
        this->p0 = p0;
        this->p1 = p1;
        this->p2 = p2;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  法線を取得
    //! 
    //! @details 領域がないときゼロベクトルを返す
    //@―---------------------------------------------------------------------------
    inline Vec3 Triangle::normal()const {
        auto tmp = Vec3::Cross(p1 - p0, p2 - p0);
        if (tmp.isZero())return Vec3::zero;
        return tmp.unitVec();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  重心を計算
    //@―---------------------------------------------------------------------------
    inline Vec3 Triangle::centerOfGravity()const noexcept {
        return (p0 + p1 + p2) / 3.0f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  内心を計算
    //@―---------------------------------------------------------------------------
    inline Vec3 Triangle::innerGravity()const noexcept {
        f32 a = Vec3::Dist(p1, p2);
        f32 b = Vec3::Dist(p2, p0);
        f32 c = Vec3::Dist(p0, p1);
        if (a + b + c < Mathf::EPSILON)return p0;
        return (p0 * a + p1 * b + p2 * c) / (a + b + c);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  外心を計算
    //@―---------------------------------------------------------------------------
    inline Vec3 Triangle::circumcenter()const noexcept {
        auto a = Vec3::Dist(p1, p2);
        auto b = Vec3::Dist(p2, p0);
        auto c = Vec3::Dist(p0, p1);
        auto ca = Vec3::CosAngle(p2 - p0, p1 - p0);
        auto cb = Vec3::CosAngle(p0 - p1, p2 - p1);
        auto cc = Vec3::CosAngle(p1 - p2, p0 - p2);
        return (a * ca * p0 + b * cb * p1 + c * cc * p2) / (a * ca + b * cb + c * cc);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  面積を計算
    //@―---------------------------------------------------------------------------
    inline f32 Triangle::area()const noexcept {
        f32 abac = Vec3::Dot(p1 - p0, p2 - p0);
        return Mathf::Sqrt(Vec3::SqrDist(p0, p1) * Vec3::SqrDist(p0, p2) - abac * abac)*0.5f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  外周を計算
    //@―---------------------------------------------------------------------------
    inline f32 Triangle::perimeter()const noexcept {
        return Vec3::Dist(p0, p1) + Vec3::Dist(p1, p2) + Vec3::Dist(p2, p0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  面を反転
    //! 
    //! @details p1 と p2 をスワップします。
    //@―---------------------------------------------------------------------------
    inline Triangle& Triangle::flip()noexcept {
        swap(p1, p2);
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      補間
    //! 
    //! @param a    開始
    //! @param b    終了
    //! @param t    補間係数
    //! @return     t=0のときa、t=1の時bを返す。
    //@―---------------------------------------------------------------------------
    inline Triangle Triangle::Lerp(const Triangle& a, const Triangle& b, f32 t)noexcept {
        return Triangle(
            Vec3::Lerp(a.p0, b.p0, t),
            Vec3::Lerp(a.p1, b.p1, t),
            Vec3::Lerp(a.p2, b.p2, t)
        );
    }



    //! @endcond
}// namespcae ob