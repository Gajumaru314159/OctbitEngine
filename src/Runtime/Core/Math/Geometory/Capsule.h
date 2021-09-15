//***********************************************************
//! @file
//! @brief		カプセル構造体
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Math/Vector/include.h>
#include <Core/Math/Quaternion.h>

namespace ob {

    //-----------------------------------------------------------------------------
    //! @brief  カプセル構造体
    //! 
    //! @note   radius が負の数の時は未定義動作です。
    //-----------------------------------------------------------------------------
    struct OB_API Capsule {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        Capsule() noexcept;                                                                         // デフォルトコンストラクタ
        Capsule(EForceInit)noexcept;                                                                // コンストラクタ(初期化あり)
        Capsule(const Vec3 pos1, const Vec3 pos2, f32 radius) noexcept;                             // コンストラクタ(2点指定)
        Capsule(const Vec3& center, f32 height, f32 radius, const Quat& quat) noexcept;             // コンストラクタ(中心/Quat指定)
        Capsule(const Vec3& center, f32 height, f32 radius, const Vec3& direction);                 // コンストラクタ(中心/方向指定)


        //===============================================================
        //  セッター
        //===============================================================
        void    Reset() noexcept;                                                                   // 構造体の初期化
        void    Set(const Vec3 pos1, const Vec3 pos2, f32 radius) noexcept;                         // セッター(2点指定)
        void    Set(const Vec3& center, f32 radius, f32 height, const Quat& quat) noexcept;         // セッター(中心/Quat指定)
        void    Set(const Vec3& center, f32 radius, f32 height, const Vec3& direction);             // セッター(中心/方向指定)

        //===============================================================
        //  ゲッター
        //===============================================================
        f32     GetHeight()const;                                                                   // 高さの取得
        f32     GetPointDistance()const;                                                            // 半径を含まない高さの取得
        Vec3    GetDirection()const;                                                                // pos1->pos2の方向ベクトルの取得

    public:

        Vec3    pos1;       //!< 始点
        Vec3    pos2;       //!< 終点
        f32     radius;     //!< 半径

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------------------------
    //! @brief          デフォルトコンストラクタ
    //-----------------------------------------------------------------------------
    Capsule::Capsule() noexcept {}


    //-----------------------------------------------------------------------------
    //! @brief          コンストラクタ(初期化あり)
    //-----------------------------------------------------------------------------
    Capsule::Capsule(EForceInit) noexcept {
        Reset();
    }


    //-----------------------------------------------------------------------------
    //! @brief          コンストラクタ(2点指定)
    //! 
    //! @param pos1     始点
    //! @param pos2     終点
    //! @param radius   半径
    //-----------------------------------------------------------------------------
    inline Capsule::Capsule(const Vec3 pos1, const Vec3 pos2, f32 radius) noexcept {
        Set(pos1, pos2, radius);
    }


    //-----------------------------------------------------------------------------
    //! @brief          コンストラクタ(中心/Quat指定)
    //! 
    //! @param center   中心点
    //! @param radius   半径
    //! @param height   カプセルの高さ(半径を含まない)
    //! @param quat     カプセルの回転(無回転でY-up方向)
    //-----------------------------------------------------------------------------
    inline Capsule::Capsule(const Vec3& center, f32 height, f32 radius, const Quat& quat) {
        Set(center, radius, height, quat);
    }

    //-----------------------------------------------------------------------------
    //! @brief           コンストラクタ(中心/方向指定)
    //!                  
    //! @param center    中心点
    //! @param radius    半径
    //! @param height    カプセルの高さ(半径を含まない)
    //! @param direction カプセルの上方向
    //-----------------------------------------------------------------------------
    inline Capsule::Capsule(const Vec3& center, f32 height, f32 radius, const Vec3& direction) {
        Set(center, radius, height, direction);
    }


    //-----------------------------------------------------------------------------
    //! @brief      状態を初期状態に戻す
    //! 
    //! @detail     center と size を0に初期化する
    //-----------------------------------------------------------------------------
    void Capsule::Reset() noexcept {
        pos1.SetZero();
        pos2.SetZero();
        radius = 0.5f;
    }


    //-----------------------------------------------------------------------------
    //! @brief          セッター(2点指定)
    //! 
    //! @param pos1     始点
    //! @param pos2     終点
    //! @param radius   半径
    //-----------------------------------------------------------------------------
    inline void Capsule::Set(const Vec3 pos1, const Vec3 pos2, f32 radius) noexcept {
        this->pos1 = pos1;
        this->pos2 = pos2;
        this->radius = radius;
    }


    //-----------------------------------------------------------------------------
    //! @brief          セッター(中心/Quat指定)
    //! 
    //! @param center   中心点
    //! @param radius   半径
    //! @param height   カプセルの高さ(半径を含まない)
    //! @param quat     カプセルの回転(無回転でY-up方向)
    //-----------------------------------------------------------------------------
    inline void Capsule::Set(const Vec3& center, f32 radius, f32 height, const Quat& quat) noexcept {
        this->radius = radius;
        const Vec3 half = quat.Up() * (height * 0.5f);
        pos1 = center + half;
        pos2 = center - half;
    }

    //-----------------------------------------------------------------------------
    //! @brief           セッター(中心/方向指定)
    //!                  
    //! @param center    中心点
    //! @param radius    半径
    //! @param height    カプセルの高さ(半径を含まない)
    //! @param direction カプセルの上方向
    //-----------------------------------------------------------------------------
    inline void Capsule::Set(const Vec3& center, f32 radius, f32 height, const Vec3& direction) {
        this->radius = radius;
        const Vec3 half = direction.GetUnitVec() * (height * 0.5f);
        pos1 = center + half;
        pos2 = center - half;
    }


    //-----------------------------------------------------------------------------
    //! @brief           高さの取得
    //!                  
    //! @details         カプセルの半径を含みません。
    //-----------------------------------------------------------------------------
    inline f32 Capsule::GetHeight()const {
        return Vec3::Dist(pos1, pos2) + radius;
    }


    //-----------------------------------------------------------------------------
    //! @brief           半径を含まない高さの取得
    //-----------------------------------------------------------------------------
    inline f32 Capsule::GetPointDistance()const {
        return Vec3::Dist(pos1, pos2);
    }


    //-----------------------------------------------------------------------------
    //! @brief           pos1->pos2の方向ベクトルの取得
    //-----------------------------------------------------------------------------
    inline Vec3 Capsule::GetDirection()const {
        return (pos2 - pos1).GetUnitVec();
    }

}// namespcae ob