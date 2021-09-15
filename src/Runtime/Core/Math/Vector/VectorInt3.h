﻿//***********************************************************
//! @file
//! @brief 整数三次元ベクトル
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Math/Mathf.h>


/*
namespace ob
{

    struct Vec3;

    //-----------------------------------------------------------------------------
    //! @brief 二次元ベクトル
    //-----------------------------------------------------------------------------
    struct OB_API Vec3Int
    {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        Vec3Int();
        Vec3Int(s32 _x, s32 _y, s32 _z);


        //===============================================================
        //  オペレータ
        //===============================================================
        Vec3Int& operator = (const Vec3Int& v);
        bool operator == (const Vec3Int& v) const;
        bool operator != (const Vec3Int& v) const;
        Vec3Int operator + (const Vec3Int& v) const;
        Vec3Int operator - (const Vec3Int& v) const;
        Vec3Int operator * (s32 f) const;
        Vec3Int operator * (const Vec3Int& v) const;
        Vec3Int operator / (s32 f) const;
        Vec3Int operator / (const Vec3Int& v) const;
        Vec3Int operator - () const;
        friend Vec3Int operator * (s32 f, const Vec3Int& v);
        friend Vec3Int operator / (s32 f, const Vec3Int& v);
        Vec3Int& operator += (const Vec3Int& v);
        Vec3Int& operator -= (const Vec3Int& v);
        Vec3Int& operator *= (s32 f);
        Vec3Int& operator /= (s32 f);
        operator Vec3 ()const;


        //===============================================================
        //  セッター
        //===============================================================
        void Set(s32 x, s32 y, s32 z);
        void SetZero();


        //===============================================================
        //  判定
        //===============================================================
        B32 IsZero();

    public:

        union
        {
            struct
            {
                s32 x;      //!< ベクトルのx成分
                s32 y;      //!< ベクトルのy成分
                s32 z;      //!< ベクトルのz成分
            };
            s32 data[3];    //!< ベクトルの成分
        };

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------------------------
    //! @brief デフォルトコンストラクタ
    //! @details 0ベクトルで初期化する。
    //-----------------------------------------------------------------------------
    inline Vec3Int::Vec3Int() :Vec3Int(0, 0, 0) {}

    //-----------------------------------------------------------------------------
    //! @brief コンストラクタ
    //-----------------------------------------------------------------------------
    inline Vec3Int::Vec3Int(s32 _x, s32 _y, s32 _z) : x(_x), y(_y), z(_z) {}

    //-----------------------------------------------------------------------------
    //! @brief 代入演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int& Vec3Int::operator = (const Vec3Int& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    //-----------------------------------------------------------------------------
    //! @brief 等価演算子
    //-----------------------------------------------------------------------------
    inline bool Vec3Int::operator == (const Vec3Int& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }

    //-----------------------------------------------------------------------------
    //! @brief 否等価演算子
    //-----------------------------------------------------------------------------
    inline bool Vec3Int::operator != (const Vec3Int& v) const
    {
        return !(operator==(v));
    }

    //-----------------------------------------------------------------------------
    //! @brief 加算演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int Vec3Int::operator + (const Vec3Int& v) const
    {
        return Vec3Int(*this) += v;
    }

    //-----------------------------------------------------------------------------
    //! @brief 減算演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int Vec3Int::operator - (const Vec3Int& v) const
    {
        return Vec3Int(*this) -= v;
    }

    //-----------------------------------------------------------------------------
    //! @brief 乗算演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int Vec3Int::operator * (s32 f) const
    {
        return Vec3Int(*this) *= f;
    }

    //-----------------------------------------------------------------------------
    //! @brief 乗算演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int Vec3Int::operator * (const Vec3Int& v) const
    {
        return Vec3Int(*this) * v;
    }

    //-----------------------------------------------------------------------------
    //! @brief 除算演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int Vec3Int::operator / (s32 f) const
    {
        return Vec3Int(*this) / f;
    }

    //-----------------------------------------------------------------------------
    //! @brief 除算演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int Vec3Int::operator / (const Vec3Int& v) const
    {
        return Vec3Int(*this) / v;
    }

    //-----------------------------------------------------------------------------
    //! @brief 単項マイナス演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int Vec3Int::operator - () const
    {
        return Vec3Int(-x, -y, -z);
    }

    //-----------------------------------------------------------------------------
    //! @brief 加算代入演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int& Vec3Int::operator += (const Vec3Int& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    //-----------------------------------------------------------------------------
    //! @brief 減算代入演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int& Vec3Int::operator -= (const Vec3Int& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    //-----------------------------------------------------------------------------
    //! @brief 乗算代入演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int& Vec3Int::operator *= (s32 f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    //-----------------------------------------------------------------------------
    //! @brief 除算代入演算子
    //-----------------------------------------------------------------------------
    inline Vec3Int& Vec3Int::operator /= (s32 f)
    {
        x /= f;
        y /= f;
        z /= f;
        return *this;
    }

    //-----------------------------------------------------------------------------
    //! @brief 要素の設定
    //-----------------------------------------------------------------------------
    inline void Vec3Int::Set(s32 x, s32 y, s32 z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    //-----------------------------------------------------------------------------
    //! @brief 要素をすべて0に設定
    //-----------------------------------------------------------------------------
    inline void Vec3Int::SetZero()
    {
        Set(0, 0, 0);
    }

    //-----------------------------------------------------------------------------
    //! @brief 要素をすべて0に設定
    //-----------------------------------------------------------------------------
    inline B32 Vec3Int::IsZero()
    {
        return x == 0 && y == 0 && z == 0;
    }
}
*/