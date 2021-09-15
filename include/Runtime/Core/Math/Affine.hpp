//***********************************************************
//! @file
//! @brief アフィン行列
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>
/*
namespace ob
{

    class Vec3;
    class Vec4;
    class Quat;
    class Affine;

    //-----------------------------------------------------------
    //! @brief アフィン行列
    //-----------------------------------------------------------
    class OB_API Affine
    {
    public:

        //-----------------------------------------------------------
        // コンストラクタ / デストラクタ
        //-----------------------------------------------------------
        Affine();                                           //!< 単位行列に初期化
        Affine(f32 m00, f32 m01, f32 m02, f32 m03,          //!< 要素を指定して初期化
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23);

        //-----------------------------------------------------------
        // オペレータ
        //-----------------------------------------------------------
        Affine& operator = (const Affine& other);           //!< Affine から初期化
        bool operator == (const Affine& v) const;           //!< 等価演算子
        bool operator != (const Affine& v) const;           //!< 否等価演算子
        Affine operator * (const Affine& other) const;      //!< 行列合成演算子
        Affine operator *= (const Affine& o);               //!< 行列合成代入演算子

        void SetIdentity();									//!< 行列に単位行列を設定
        void Set(f32 m00, f32 m01, f32 m02, f32 m03,		//!< 行列値の設定
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23);


        void Translate(f32 x, f32 y, f32 z);				//!< 行列に平行移動行列を乗算
        void Translate(const Vec3& pos);					//!< 行列に平行移動行列を乗算
        void Rotate(f32 x, f32 y, f32 z);					//!< 行列に回転行列を乗算
        void Rotate(const Vec3& rot);					    //!< 行列に回転行列を乗算
        void Rotate(const Quat& quat);				        //!< 行列に回転行列を乗算
        void Scale(f32 x, f32 y, f32 z);					//!< 行列に拡大縮小行列を乗算
        void Scale(const Vec3& scale);					    //!< 行列に拡大縮小行列を乗算


        f32		Matrix()const;						        //!< 行列式の計算
        Affine	GetInverse()const;							//!< 逆行列の計算

        Vec3 GetTrans()const;							    //!< 行列から移動量を計算
        Vec3 GetRotation()const;							//!< 行列から回転量を計算
        Vec3 GetScale()const;							    //!< 行列から拡大量を計算
        Quat GetQuat()const;							    //!< 行列から回転量を計算

    public:

        f32 m[3][4];//!< 内部データ

    private:

        static const s32 COL;//! 4
        static const s32 ROW;//! 3

    };





    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------
    //! @brief		単位行列に初期化
    //! @details	行列を単位行列で初期化する。
    //-----------------------------------------------------------
    inline Affine::Affine()
    {
        SetIdentity();
    }


    //-----------------------------------------------------------
    //! @brief 行列に単位行列を設定
    //-----------------------------------------------------------
    inline void Affine::SetIdentity()
    {
        memset(m, 0, sizeof(m));
        m[0][0] = 1;
        m[1][1] = 1;
        m[2][2] = 1;
    }


    //-----------------------------------------------------------
    //! @brief 行列に平行移動行列を乗算
    //! @param x X軸方向の移動量
    //! @param y Y軸方向の移動量
    //! @param z Z軸方向の移動量
    //-----------------------------------------------------------
    inline void Affine::Translate(const f32 x, const f32 y, const f32 z)
    {
        Affine af(
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z);
        (*this) *= af;
    }


    //-----------------------------------------------------------
    //! @brief 行列に拡大縮小行列を乗算
    //! @param x X軸方向の拡大縮小量
    //! @param y Y軸方向の拡大縮小量
    //! @param z Z軸方向の拡大縮小量
    //-----------------------------------------------------------
    inline void Affine::Scale(const f32 x, const f32 y, const f32 z)
    {
        Affine af(
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0);
        (*this) *= af;
    }

}// namespace ob
*/