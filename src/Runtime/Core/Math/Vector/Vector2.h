﻿//***********************************************************
//! @file
//! @brief 四次元ベクトル
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Math/Mathf.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief 二次元ベクトル
    //@―---------------------------------------------------------------------------
    struct OB_API Vec2 {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Vec2()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(ゼロ初期化)
        //@―---------------------------------------------------------------------------
        explicit Vec2(EForceInit)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
        //@―---------------------------------------------------------------------------
        explicit Vec2(f32 scalar)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(要素を指定して初期化)
        //@―---------------------------------------------------------------------------
        Vec2(f32 x, f32 y)noexcept;


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      等価演算子 
        //! 
        //! @details    2つのベクトルの差が Mathf::TOLERANCE より小さい場合、2つのベクトルは等しいと
        //!             見なされます。
        //@―---------------------------------------------------------------------------
        bool operator == (const Vec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子
        //! 
        //! @details    2つのベクトルの差が Mathf::TOLERANCE より大きい場合、2つのベクトルは異なると
        //!             見なされます。
        //@―---------------------------------------------------------------------------
        bool operator != (const Vec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子
        //@―---------------------------------------------------------------------------
        Vec2 operator + (const Vec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Vec2 operator + (f32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子
        //@―---------------------------------------------------------------------------
        Vec2 operator - (const Vec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Vec2 operator - (f32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子
        //@―---------------------------------------------------------------------------
        Vec2 operator * (const Vec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Vec2 operator * (f32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子
        //@―---------------------------------------------------------------------------
        Vec2 operator / (const Vec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Vec2 operator / (f32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子
        //@―---------------------------------------------------------------------------
        Vec2& operator += (const Vec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Vec2& operator += (f32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子 
        //@―---------------------------------------------------------------------------
        Vec2& operator -= (const Vec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Vec2& operator -= (f32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子 
        //@―---------------------------------------------------------------------------
        Vec2& operator *= (const Vec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子 (スカラー)
        //@―---------------------------------------------------------------------------
        Vec2& operator *= (f32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子 
        //@―---------------------------------------------------------------------------
        Vec2& operator /= (const Vec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        Vec2& operator /= (f32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 単項マイナス演算子
        //@―---------------------------------------------------------------------------
        Vec2 operator - () const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief f32*Vec2演算子 
        //@―---------------------------------------------------------------------------
        friend Vec2 operator * (f32 scalar, const Vec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief f32/Vec2演算子 
        //@―---------------------------------------------------------------------------
        friend Vec2 operator / (f32 scalar, const Vec2& v) noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 要素の設定
        //@―---------------------------------------------------------------------------
        void    Set(f32 x, f32 y) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素の設定
        //@―---------------------------------------------------------------------------
        void    Set(const Vec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素を全て0に設定
        //@―---------------------------------------------------------------------------
        void    SetZero() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 全ての要素を同じ値で設定
        //@―---------------------------------------------------------------------------
        void    SetAll(f32 scalar) noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================
        // 
        //@―---------------------------------------------------------------------------
        //! @brief 要素の最大を取得
        //@―---------------------------------------------------------------------------
        f32     GetMaxComponent() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素の絶対値の最大を取得
        //@―---------------------------------------------------------------------------
        f32     GetMaxAbsComponent() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素の最小を取得
        //@―---------------------------------------------------------------------------
        f32     GetMinComponent() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素の絶対値の最小を取得
        //@―---------------------------------------------------------------------------
        f32     GetMinAbsComponent() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief ベクトルの長さの取得
        //! 
        //! @details 二つのベクトルの長さを比較する場合はSqrMagのほうが高速です。
        //@―---------------------------------------------------------------------------
        f32     GetMag() const;


        //@―---------------------------------------------------------------------------
        //! @brief ベクトルの長さの二乗を取得
        //@―---------------------------------------------------------------------------
        f32     GetSqrMag() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief              単位ベクトルの取得
        //! 
        //! @details            0ベクトルの場合は0ベクトルを返す。
        //! @param tolerance    ゼロベクトル判定の誤差の許容誤差
        //@―---------------------------------------------------------------------------
        Vec2    GetUnitVec(f32 tolerance = Mathf::TOLERANCE)const;


        //@―---------------------------------------------------------------------------
        //! @brief              単位ベクトルの取得(ゼロチェックなし)
        //@―---------------------------------------------------------------------------
        Vec2    GetUnitVecUnsafe()const;


        //@―---------------------------------------------------------------------------
        //! @brief              方向ベクトルとベクトルの長さの取得
        //! 
        //! @param[out] outDir  方向ベクトルの出力
        //! @param[out] outLen  ベクトルの大きさ
        //@―---------------------------------------------------------------------------
        void    ToDirectionAndLength(Vec2& outDir, f32& outLen)const;


        //===========================================================
        //  変換
        //===========================================================

        //@―---------------------------------------------------------------------------
        //! @brief              ベクトルの正規化
        //! 
        //! @details            0ベクトルの場合は何もしない。
        //! @param tolerance    ゼロベクトル判定の誤差の許容誤差
        //@―---------------------------------------------------------------------------
        void     Normalize(f32 tolerance = Mathf::TOLERANCE);


        //@―---------------------------------------------------------------------------
        //! @brief              ベクトルの正規化(ゼロチェックなし)
        //@―---------------------------------------------------------------------------
        void     NormalizeUnsafe();


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 単位ベクトルか判定
        //@―---------------------------------------------------------------------------
        b32     IsUnit(f32 tolerance = Mathf::TOLERANCE)const;


        //@―---------------------------------------------------------------------------
        //! @brief 等価判定(許容誤差指定)
        //! 
        //! @param v            比較対象
        //! @param tolerance    許容誤差
        //@―---------------------------------------------------------------------------
        b32     Equals(const Vec2& v, f32 tolerance = Mathf::TOLERANCE)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief すべての要素が同じ値か判定
        //! 
        //! @param tolerance    許容誤差
        //@―---------------------------------------------------------------------------
        b32     AllComponentsEqual(f32 tolerance = Mathf::TOLERANCE)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief ゼロベクトルか判定
        //@―---------------------------------------------------------------------------
        b32     IsZero(f32 tolerance = Mathf::TOLERANCE)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素にNaNを含むか判定
        //@―---------------------------------------------------------------------------
        b32     IsNaN()const;


        //@―---------------------------------------------------------------------------
        //! @brief 要素にInfを含むか判定
        //@―---------------------------------------------------------------------------
        b32     IsInf()const;


        //@―---------------------------------------------------------------------------
        //! @brief 要素にNaNまたはInfを含むか判定
        //@―---------------------------------------------------------------------------
        b32     IsNaNOrInf()const;


    public:

        //===============================================================
        //  スタティック関数
        //===============================================================
        
        //@―---------------------------------------------------------------------------
        //! @brief      ベクトルの各要素の絶対値からベクトルを作成
        //@―---------------------------------------------------------------------------
        static Vec2 Abs(const Vec2& v)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      内積
        //@―---------------------------------------------------------------------------
        static f32  Dot(const Vec2& a, const Vec2& b)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      外積
        //@―---------------------------------------------------------------------------
        static f32  Cross(const Vec2& a, const Vec2& b)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      二つのベクトルの距離を計算
        //@―---------------------------------------------------------------------------
        static f32  Dist(const Vec2& a, const Vec2& b);


        //@―---------------------------------------------------------------------------
        //! @brief      二つのベクトルの距離の二乗を計算
        //@―---------------------------------------------------------------------------
        static f32  SqrDist(const Vec2& a, const Vec2& b)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      二つのベクトルのマンハッタン距離を計算
        //@―---------------------------------------------------------------------------
        static f32  MDist(const Vec2& a, const Vec2& b)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      二つのベクトルの線形補間
        //! 
        //! @param a    開始ベクトル
        //! @param b    終了ベクトル
        //! @param t    補間係数
        //! @return     t=0のときa、t=1の時bを返す。
        //@―---------------------------------------------------------------------------
        static Vec2 Lerp(const Vec2& a, const Vec2& b, f32 t)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      二つのベクトルの球面補間
        //! 
        //! @param a    開始ベクトル
        //! @param b    終了ベクトル
        //! @param t    補間係数
        //! @return     t=0のときa、t=1の時bを返す。
        //@―---------------------------------------------------------------------------
        static Vec2 Slerp(const Vec2& a, const Vec2& b, f32 t);


        //@―---------------------------------------------------------------------------
        //! @brief      二つのベクトルの各要素の大きいほうからベクトルを生成
        //@―---------------------------------------------------------------------------
        static Vec2 Max(const Vec2& a, const Vec2& b)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      二つのベクトルの各要素の小さいほうからベクトルを生成
        //@―---------------------------------------------------------------------------
        static Vec2 Min(const Vec2& a, const Vec2& b)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 二つのベクトルのなす角のコサインを取得
        //@―---------------------------------------------------------------------------
        static f32  CosAngle(const Vec2& a, const Vec2& b);


        //@―---------------------------------------------------------------------------
        //! @brief 二つのベクトルのなす角を取得
        //@―---------------------------------------------------------------------------
        static f32  Angle(const Vec2& a, const Vec2& b);


    public:

        static const Vec2 up;      //!< ( 0.0f, 1.0f)
        static const Vec2 down;    //!< ( 0.0f,-1.0f)
        static const Vec2 left;    //!< (-1.0f, 0.0f)
        static const Vec2 right;   //!< ( 1.0f, 0.0f)
        static const Vec2 zero;    //!< ( 0.0f, 0.0f)
        static const Vec2 one;     //!< ( 1.0f, 1.0f)

    public:

        f32 x;    //!< ベクトルのX成分
        f32 y;    //!< ベクトルのY成分

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(ゼロ初期化)
    //@―---------------------------------------------------------------------------
    inline Vec2::Vec2()noexcept {
        SetZero();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
    //@―---------------------------------------------------------------------------
    inline Vec2::Vec2(f32 scalar)noexcept {
        SetAll(scalar);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(要素を指定して初期化)
    //@―---------------------------------------------------------------------------
    inline Vec2::Vec2(f32 x, f32 y)noexcept {
        Set(x, y);
    }


    //===============================================================
    //  オペレータ
    //===============================================================


    //@―---------------------------------------------------------------------------
    //! @brief      等価演算子 
    //! 
    //! @details    2つのベクトルの差が Mathf::TOLERANCE より小さい場合、2つのベクトルは等しいと見なされます。
    //@―---------------------------------------------------------------------------
    inline bool Vec2::operator == (const Vec2& v) const noexcept {
        return Mathf::Approximately(x, v.x) &&
            Mathf::Approximately(y, v.y);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //! 
    //! @details    2つのベクトルの差が Mathf::TOLERANCE より大きい場合、2つのベクトルは異なると見なされます。
    //@―---------------------------------------------------------------------------
    inline bool Vec2::operator != (const Vec2& v) const noexcept {
        return !(operator==(v));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::operator + (const Vec2& v) const noexcept {
        return Vec2(*this) += v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::operator + (f32 scalar) const noexcept {
        return Vec2(*this) += scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::operator - (const Vec2& v) const noexcept {
        return Vec2(*this) -= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::operator - (f32 scalar) const noexcept {
        return Vec2(*this) -= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::operator * (const Vec2& v) const noexcept {
        return Vec2(*this) *= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::operator * (f32 scalar) const noexcept {
        return Vec2(*this) *= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  除算演算子
    //! 
    //! @note   0除算は未定義動作です。
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::operator / (const Vec2& v) const noexcept {
        return Vec2(*this) /= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  除算演算子(スカラー)
    //! 
    //! @note   0除算は未定義動作です。
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::operator / (f32 scalar) const noexcept {
        return Vec2(*this) /= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子
    //@―---------------------------------------------------------------------------
    inline Vec2& Vec2::operator += (const Vec2& v)noexcept {
        x += v.x;
        y += v.y;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline Vec2& Vec2::operator += (f32 scalar)noexcept {
        x += scalar;
        y += scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子 
    //@―---------------------------------------------------------------------------
    inline Vec2& Vec2::operator -= (const Vec2& v)noexcept {
        x -= v.x;
        y -= v.y;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline Vec2& Vec2::operator -= (f32 scalar)noexcept {
        x -= scalar;
        y -= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子 
    //@―---------------------------------------------------------------------------
    inline Vec2& Vec2::operator *= (const Vec2& v)noexcept {
        x *= v.x;
        y *= v.y;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子 (スカラー)
    //@―---------------------------------------------------------------------------
    inline Vec2& Vec2::operator *= (f32 scalar)noexcept {
        x *= scalar;
        y *= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  除算代入演算子 
    //! 
    //! @note   0除算は未定義動作です。
    //@―---------------------------------------------------------------------------
    inline Vec2& Vec2::operator /= (const Vec2& v)noexcept {
        x /= v.x;
        y /= v.y;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  除算代入演算子(スカラー)
    //! 
    //! @note   0除算は未定義動作です。
    //@―---------------------------------------------------------------------------
    inline Vec2& Vec2::operator /= (f32 scalar)noexcept {
        x /= scalar;
        y /= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 単項マイナス演算子
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::operator - () const noexcept {
        return Vec2(-x, -y);
    }


    //===============================================================
    //  セッター
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 要素の設定
    //@―---------------------------------------------------------------------------
    inline void Vec2::Set(f32 x, f32 y)noexcept {
        this->x = x;
        this->y = y;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の設定
    //@―---------------------------------------------------------------------------
    inline void Vec2::Set(const Vec2& v)noexcept {
        Set(v.x, v.y);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を全て0に設定
    //@―---------------------------------------------------------------------------
    inline void Vec2::SetZero()noexcept {
        Set(0.0f, 0.0f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 全ての要素を同じ値で設定
    //@―---------------------------------------------------------------------------
    inline void Vec2::SetAll(f32 scalar)noexcept {
        Set(scalar, scalar);
    }


    //===============================================================
    //  ゲッター
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 要素の最大を取得
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::GetMaxComponent() const noexcept {
        return Mathf::Max(x, y);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の絶対値の最大を取得
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::GetMaxAbsComponent() const noexcept {
        return Mathf::Max(Mathf::Abs(x), Mathf::Abs(y));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の最小を取得
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::GetMinComponent() const noexcept {
        return Mathf::Min(x, y);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の絶対値の最小を取得
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::GetMinAbsComponent() const noexcept {
        return Mathf::Min(Mathf::Abs(x), Mathf::Abs(y));
    }


    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの長さの取得
    //! 
    //! @details 二つのベクトルの長さを比較する場合はSqrMagのほうが高速です。
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::GetMag() const {
        return Mathf::Sqrt(GetSqrMag());
    }


    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの長さの二乗を取得
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::GetSqrMag() const noexcept {
        return x * x + y * y;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              単位ベクトルの取得
    //! 
    //! @details            0ベクトルの場合は0ベクトルを返す。
    //! @param tolerance    ゼロベクトル判定の誤差の許容誤差
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::GetUnitVec(f32 tolerance)const {
        Vec2 unitVec = *this;
        unitVec.Normalize(tolerance);
        return unitVec;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              単位ベクトルの取得(ゼロチェックなし)
    //! 
    //! @note               0ベクトルの単位ベクトルを取得しようとした場合、
    //!                     0除算が発生して未定義動作になります。
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::GetUnitVecUnsafe()const {
        Vec2 unitVec = *this;
        unitVec.NormalizeUnsafe();
        return unitVec;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              方向ベクトルとベクトルの長さの取得
    //! 
    //! @details            0ベクトルの場合 outLen には0ベクトルが入ります。
    //! @param[out] outDir  方向ベクトルの出力
    //! @param[out] outLen  ベクトルの大きさ
    //@―---------------------------------------------------------------------------
    inline void Vec2::ToDirectionAndLength(Vec2& outDir, f32& outLen)const {
        if (IsZero()) {
            outDir.SetZero();
            outLen = 0.0f;
            return;
        }
        outLen = GetSqrMag();
        if (outLen < Mathf::TOLERANCE) {
            outDir = Vec2::zero;
        } else {
            outDir = (*this) / outLen;
        }
    }


    //===========================================================
    //  変換
    //===========================================================

    //@―---------------------------------------------------------------------------
    //! @brief              ベクトルの正規化
    //! 
    //! @details            0ベクトルの場合は何もしない。
    //! @param tolerance    ゼロベクトル判定の誤差の許容誤差
    //@―---------------------------------------------------------------------------
    inline void Vec2::Normalize(f32 tolerance) {
        f32 f = GetMag();
        // 0ベクトルの場合は何もしない
        if (f <= tolerance)return;
        this->operator/=(f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief              ベクトルの正規化(ゼロチェックなし)
    //! 
    //! @note               0ベクトルを正規化した場合は未定義動作です。
    //@―---------------------------------------------------------------------------
    inline void Vec2::NormalizeUnsafe() {
        f32 f = GetMag();
        this->operator/=(f);
    }


    //===============================================================
    //  判定
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 単位ベクトルか判定
    //@―---------------------------------------------------------------------------
    inline b32 Vec2::IsUnit(f32 tolerance)const {
        const f32 mag = GetMag();
        return Mathf::Abs(mag - 1.0f) < tolerance;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価判定(許容誤差指定)
    //! 
    //! @param tolerance    許容誤差
    //@―---------------------------------------------------------------------------
    inline b32 Vec2::Equals(const Vec2& v, f32 tolerance)const noexcept {
        return
            Mathf::Abs(x - v.x) <= tolerance &&
            Mathf::Abs(y - v.y) <= tolerance;
    }


    //@―---------------------------------------------------------------------------
    //! @brief すべての要素が同じ値か判定
    //! 
    //! @param tolerance    許容誤差
    //@―---------------------------------------------------------------------------
    inline b32 Vec2::AllComponentsEqual(f32 tolerance)const noexcept {
        if (tolerance < Mathf::Abs(x - y))return false;
        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ゼロベクトルか判定
    //@―---------------------------------------------------------------------------
    inline b32 Vec2::IsZero(f32 tolerance)const noexcept {
        if (tolerance < Mathf::Abs(x))return false;
        if (tolerance < Mathf::Abs(y))return false;
        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素にNaNを含むか判定
    //@―---------------------------------------------------------------------------
    inline b32 Vec2::IsNaN()const {
        if (Mathf::IsNaN(x))return true;
        if (Mathf::IsNaN(y))return true;
        return false;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素にInfを含むか判定
    //@―---------------------------------------------------------------------------
    inline b32 Vec2::IsInf()const {
        if (Mathf::IsInf(x))return true;
        if (Mathf::IsInf(y))return true;
        return false;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素にNaNまたはInfを含むか判定
    //@―---------------------------------------------------------------------------
    inline b32 Vec2::IsNaNOrInf()const {
        return IsNaN() || IsInf();
    }


    //===============================================================
    //  スタティック関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの各要素の絶対値からベクトルを作成
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::Abs(const Vec2& v)noexcept {
        return Vec2(Mathf::Abs(v.x), Mathf::Abs(v.y));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 内積
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::Dot(const Vec2& a, const Vec2& b)noexcept {
        return a.x * b.x + a.y * b.y;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 外積
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::Cross(const Vec2& a, const Vec2& b)noexcept {
        return a.x * b.y - a.y * b.x;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  二つのベクトルの距離を計算
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::Dist(const Vec2& a, const Vec2& b) {
        return (b - a).GetMag();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  二つのベクトルの距離の二乗を計算
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::SqrDist(const Vec2& a, const Vec2& b)noexcept {
        return (b - a).GetSqrMag();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  二つのベクトルのマンハッタン距離を計算
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::MDist(const Vec2& a, const Vec2& b)noexcept {
        return Mathf::Abs(a.x - b.x) + Mathf::Abs(a.y - b.y);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの線形補間
    //! 
    //! @param a    開始ベクトル
    //! @param b    終了ベクトル
    //! @param t    補間係数
    //! @return     t=0のときa、t=1の時bを返す。
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::Lerp(const Vec2& a, const Vec2& b, f32 t)noexcept {
        return Vec2(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t
        );
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの球面補間
    //! 
    //! @param a    開始ベクトル
    //! @param b    終了ベクトル
    //! @param t    補間係数
    //! @return     t=0のときa、t=1の時bを返す。
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::Slerp(const Vec2& a, const Vec2& b, f32 t) {
        f32 al, bl;
        Vec2 an, bn;
        a.ToDirectionAndLength(an, al);
        b.ToDirectionAndLength(bn, bl);
        f32 angle = Mathf::Acos(Dot(an, bn));
        f32 sinTh = Mathf::Sin(angle);
        f32 ps = Mathf::Sin(angle * (1.0f - t)) * Mathf::Lerp(al, bl, t);
        f32 pe = Mathf::Sin(angle * t) * Mathf::Lerp(al, bl, t);

        return Vec2(
            an.x * ps + bn.x * pe,
            an.y * ps + bn.y * pe
        );
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::Max(const Vec2& a, const Vec2& b)noexcept {
        return Vec2(Mathf::Max(a.x, b.x), Mathf::Max(a.y, b.y));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
    //@―---------------------------------------------------------------------------
    inline Vec2 Vec2::Min(const Vec2& a, const Vec2& b)noexcept {
        return Vec2(Mathf::Min(a.x, b.x), Mathf::Min(a.y, b.y));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルのなす角のコサインを取得
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::CosAngle(const Vec2& a, const Vec2& b) {
        return Dot(a.GetUnitVec(), b.GetUnitVec());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルのなす角を取得
    //@―---------------------------------------------------------------------------
    inline f32 Vec2::Angle(const Vec2& a, const Vec2& b) {
        return Mathf::AcosD(CosAngle(a, b));
    }

    //! @endcond
}// namespace ob
