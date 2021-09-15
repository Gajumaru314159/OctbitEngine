//***********************************************************
//! @file
//! @brief 四次元ベクトル
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Math/Mathf.hpp>

namespace ob
{

    //-----------------------------------------------------------
    //! @brief 四次元ベクトル
    //-----------------------------------------------------------
    struct OB_API Vec4
    {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        Vec4();                                             // コンストラクタ(ゼロ初期化)
        explicit Vec4(f32 scalar);                          // コンストラクタ(すべての要素を同じ値で初期化)
        Vec4(f32 x, f32 y, f32 z, f32 w);                   // コンストラクタ(要素を指定して初期化)


        //===============================================================
        //  オペレータ
        //===============================================================
        bool operator == (const Vec4& v) const;             // 等価演算子
        bool operator != (const Vec4& v) const;             // 否等価演算子

        Vec4 operator + (const Vec4& v) const;              // 加算演算子
        Vec4 operator + (f32 scalar) const;                 // 加算演算子(スカラー)
        Vec4 operator - (const Vec4& v) const;              // 減算演算子
        Vec4 operator - (f32 scalar) const;                 // 減算演算子(スカラー)
        Vec4 operator * (const Vec4& v) const;              // 乗算演算子
        Vec4 operator * (f32 scalar) const;                 // 乗算演算子(スカラー)
        friend Vec4 operator * (f32 scalar, const Vec4& v); // 乗算演算子(前方スカラー)
        Vec4 operator / (const Vec4& v) const;              // 除算演算子
        Vec4 operator / (f32 scalar) const;                 // 除算演算子(スカラー)
        friend Vec4 operator / (f32 scalar, const Vec4& v); // 除算演算子(前方スカラー)

        Vec4& operator += (const Vec4& v);                  // 加算代入演算子
        Vec4& operator += (f32 scalar);                     // 加算代入演算子(スカラー)
        Vec4& operator -= (const Vec4& v);                  // 減算代入演算子
        Vec4& operator -= (f32 scalar);                     // 減算代入演算子(スカラー)
        Vec4& operator *= (const Vec4& v);                  // 乗算代入演算子
        Vec4& operator *= (f32 scalar);                     // 乗算代入演算子(スカラー)
        Vec4& operator /= (const Vec4& v);                  // 除算代入演算子
        Vec4& operator /= (f32 scalar);                     // 除算代入演算子(スカラー)

        Vec4 operator - () const;                           // 単項マイナス演算子


        //===============================================================
        //  セッター
        //===============================================================;
        void    Set(f32 x, f32 y, f32 z, f32 w);            // 要素の設定
        void    Set(const Vec4& v);                         // 要素の設定
        void    SetZero();                                  // 要素を全て0に設定
        void    SetAll(f32 scalar);                         // 全ての要素を同じ値で設定


        //===============================================================
        //  ゲッター
        //===============================================================
        f32     MaxComponent() const;                                           // 要素の最大を取得
        f32     MaxAbsComponent() const;                                        // 要素の絶対値の最大を取得
        f32     MinComponent() const;                                           // 要素の最小を取得
        f32     MinAbsComponent() const;                                        // 要素の絶対値の最小を取得
        f32     Mag() const;                                                    // ベクトルの長さの取得
        f32     SqrMag() const;                                                 // ベクトルの長さの二乗を取得
        Vec4    UnitVec(f32 tolerance = Mathf::TOLERANCE)const;                 // 単位ベクトルの取得
        Vec4    UnitVecUnsafe()const;                                           // 単位ベクトルの取得(ゼロチェックなし)
        void    ToDirectionAndLength(Vec4& outDir, f32& outLen)const;           // 方向ベクトルとベクトルの長さの取得


        //===========================================================
        //  変換
        //===========================================================
        void     Normalize(f32 tolerance = Mathf::TOLERANCE);                   // ベクトルの正規化
        void     NormalizeUnsafe();                                             // ベクトルの正規化(ゼロチェックなし)


        //===============================================================
        //  判定
        //===============================================================
        b32     IsUnit(f32 tolerance = Mathf::TOLERANCE)const;                  // 単位ベクトルか判定
        b32     Equals(const Vec4& v, f32 tolerance = Mathf::TOLERANCE)const;   // 等価判定(許容範囲指定)
        b32     AllComponentsEqual(f32 tolerance = Mathf::TOLERANCE)const;      // すべての要素が同じ値か判定
        b32     IsZero(f32 tolerance = Mathf::TOLERANCE)const;                  // ゼロベクトルか判定
        b32     IsNaN()const;                                                   // 要素にNaNを含むか判定

    public:

        //===============================================================
        //  スタティック関数
        //===============================================================
        static Vec4 Abs(const Vec4& v);                                         // ベクトルの各要素の絶対値からベクトルを作成
        static f32  Dot(const Vec4& a, const Vec4& b);                          // 内積
        static Vec4 Cross(const Vec4& a, const Vec4& b);                        // 外積
        static f32  Dist(const Vec4& a, const Vec4& b);                         // 二つのベクトルの距離を計算
        static f32  SqrDist(const Vec4& a, const Vec4& b);                      // 二つのベクトルの距離の二乗を計算
        static f32  MDist(const Vec4& a, const Vec4& b);                        // 二つのベクトルのマンハッタン距離を計算
        static Vec4 Lerp(const Vec4& a, const Vec4& b, f32 t);                  // 二つのベクトルの線形補間
        static Vec4 Slerp(const Vec4& a, const Vec4& b, f32 t);                 // 二つのベクトルの球面補間
        static Vec4 Max(const Vec4& a, const Vec4& b);                          // 二つのベクトルの各要素の大きいほうからベクトルを生成
        static Vec4 Min(const Vec4& a, const Vec4& b);                          // 二つのベクトルの各要素の小さいほうからベクトルを生成
        static f32  CosAngle(const Vec4& a, const Vec4& b);                     // 二つのベクトルのなす角のコサインを計算
        static f32  Angle(const Vec4& a, const Vec4& b);                        // 二つのベクトルのなす角を計算


    public:

        static const Vec4 zero;    //!< ( 0.0f, 0.0f, 0.0f, 0.0f)
        static const Vec4 one;     //!< ( 1.0f, 1.0f, 1.0f, 1.0f)

    public:

        union
        {
            struct
            {
                f32 x;    //!< ベクトルのX成分
                f32 y;    //!< ベクトルのY成分
                f32 z;    //!< ベクトルのZ成分
                f32 w;    //!< ベクトルのW成分
            };
            f32 data[4];  //!< ベクトルの成分
        };

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------
    //! @brief      コンストラクタ(ゼロ初期化)
    //-----------------------------------------------------------
    inline Vec4::Vec4() {
        SetZero();
    }


    //-----------------------------------------------------------
    //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
    //-----------------------------------------------------------
    inline Vec4::Vec4(f32 scalar)
    {
        Set(scalar, scalar, scalar, scalar);
    }


    //-----------------------------------------------------------
    //! @brief      コンストラクタ(要素を指定して初期化)
    //-----------------------------------------------------------
    inline Vec4::Vec4(f32 x, f32 y, f32 z, f32 w)
    {
        Set(x, y, z, w);
    }


    //===============================================================
    //  オペレータ
    //===============================================================


    //-----------------------------------------------------------
    //! @brief      等価演算子 
    //! 
    //! @details    2つのベクトルの差が 1e-5 より小さい場合、2つのベクトルは等しいと見なされます。
    //-----------------------------------------------------------
    inline bool Vec4::operator == (const Vec4& v) const
    {
        return Mathf::Approximately(x, v.x) &&
            Mathf::Approximately(y, v.y) &&
            Mathf::Approximately(z, v.z) &&
            Mathf::Approximately(w, v.w);
    }


    //-----------------------------------------------------------
    //! @brief 否等価演算子
    //! 
    //! @details    2つのベクトルの差が 1e-5 より大きい場合、2つのベクトルは異なると見なされます。
    //-----------------------------------------------------------
    inline bool Vec4::operator != (const Vec4& v) const
    {
        return !(operator==(v));
    }


    //-----------------------------------------------------------
    //! @brief 加算演算子
    //-----------------------------------------------------------
    inline Vec4 Vec4::operator + (const Vec4& v) const
    {
        return Vec4(*this) += v;
    }


    //-----------------------------------------------------------
    //! @brief 加算演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec4 Vec4::operator + (f32 scalar) const
    {
        return Vec4(*this) += scalar;
    }


    //-----------------------------------------------------------
    //! @brief 減算演算子
    //-----------------------------------------------------------
    inline Vec4 Vec4::operator - (const Vec4& v) const
    {
        return Vec4(*this) -= v;
    }


    //-----------------------------------------------------------
    //! @brief 減算演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec4 Vec4::operator - (f32 scalar) const
    {
        return Vec4(*this) -= scalar;
    }


    //-----------------------------------------------------------
    //! @brief 乗算演算子
    //-----------------------------------------------------------
    inline Vec4 Vec4::operator * (const Vec4& v) const
    {
        return Vec4(*this) *= v;
    }


    //-----------------------------------------------------------
    //! @brief 乗算演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec4 Vec4::operator * (f32 scalar) const
    {
        return Vec4(*this) *= scalar;
    }


    //-----------------------------------------------------------
    //! @brief 除算演算子
    //-----------------------------------------------------------
    inline Vec4 Vec4::operator / (const Vec4& v) const
    {
        return Vec4(*this) /= v;
    }


    //-----------------------------------------------------------
    //! @brief 除算演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec4 Vec4::operator / (f32 scalar) const
    {
        return Vec4(*this) /= scalar;
    }


    //-----------------------------------------------------------
    //! @brief 加算代入演算子
    //-----------------------------------------------------------
    inline Vec4& Vec4::operator += (const Vec4& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 加算代入演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec4& Vec4::operator += (f32 scalar)
    {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 減算代入演算子 
    //-----------------------------------------------------------
    inline Vec4& Vec4::operator -= (const Vec4& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 減算代入演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec4& Vec4::operator -= (f32 scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 乗算代入演算子 
    //-----------------------------------------------------------
    inline Vec4& Vec4::operator *= (const Vec4& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 乗算代入演算子 (スカラー)
    //-----------------------------------------------------------
    inline Vec4& Vec4::operator *= (f32 scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 除算代入演算子 
    //-----------------------------------------------------------
    inline Vec4& Vec4::operator /= (const Vec4& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 除算代入演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec4& Vec4::operator /= (f32 scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 単項マイナス演算子
    //-----------------------------------------------------------
    inline Vec4 Vec4::operator - () const
    {
        return Vec4(-x, -y, -z, -w);
    }


    //===============================================================
    //  セッター
    //===============================================================

    //-----------------------------------------------------------
    //! @brief 要素の設定
    //-----------------------------------------------------------
    inline void Vec4::Set(f32 x, f32 y, f32 z, f32 w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }


    //-----------------------------------------------------------
    //! @brief 要素の設定
    //-----------------------------------------------------------
    inline void Vec4::Set(const Vec4& v)
    {
        Set(v.x, v.y, v.z, v.w);
    }


    //-----------------------------------------------------------
    //! @brief 要素を全て0に設定
    //-----------------------------------------------------------
    inline void Vec4::SetZero()
    {
        Set(0.0f, 0.0f, 0.0f, 0.0f);
    }


    //-----------------------------------------------------------
    //! @brief 全ての要素を同じ値で設定
    //-----------------------------------------------------------
    inline void Vec4::SetAll(f32 scalar)
    {
        Set(scalar, scalar, scalar, scalar);
    }


    //===============================================================
    //  ゲッター
    //===============================================================

    //-----------------------------------------------------------
    //! @brief 要素の最大を取得
    //-----------------------------------------------------------
    inline f32 Vec4::MaxComponent() const
    {
        return Mathf::Max(x, y, z, w);
    }


    //-----------------------------------------------------------
    //! @brief 要素の絶対値の最大を取得
    //-----------------------------------------------------------
    inline f32 Vec4::MaxAbsComponent() const
    {
        return Mathf::Max(Mathf::Abs(x), Mathf::Abs(y), Mathf::Abs(z), Mathf::Abs(w));
    }


    //-----------------------------------------------------------
    //! @brief 要素の最小を取得
    //-----------------------------------------------------------
    inline f32 Vec4::MinComponent() const
    {
        return Mathf::Min(x, y, z, w);
    }


    //-----------------------------------------------------------
    //! @brief 要素の絶対値の最小を取得
    //-----------------------------------------------------------
    inline f32 Vec4::MinAbsComponent() const
    {
        return Mathf::Min(Mathf::Abs(x), Mathf::Abs(y), Mathf::Abs(z), Mathf::Abs(w));
    }


    //-----------------------------------------------------------
    //! @brief ベクトルの長さの取得
    //! 
    //! @details 二つのベクトルの長さを比較する場合はSqrMagのほうが高速です。
    //-----------------------------------------------------------
    inline f32 Vec4::Mag() const
    {
        return Mathf::Sqrt(SqrMag());
    }


    //-----------------------------------------------------------
    //! @brief ベクトルの長さの二乗を取得
    //-----------------------------------------------------------
    inline f32 Vec4::SqrMag() const
    {
        return x * x + y * y + z * z + w * w;
    }


    //-----------------------------------------------------------
    //! @brief              単位ベクトルの取得
    //! 
    //! @details            0ベクトルの場合は0ベクトルを返す。
    //! @param tolerance    ゼロベクトル判定の誤差の許容範囲
    //-----------------------------------------------------------
    inline Vec4 Vec4::UnitVec(f32 tolerance)const
    {
        Vec4 unitVec = *this;
        unitVec.Normalize(tolerance);
        return unitVec;
    }


    //-----------------------------------------------------------
    //! @brief              単位ベクトルの取得(ゼロチェックなし)
    //-----------------------------------------------------------
    inline Vec4 Vec4::UnitVecUnsafe()const
    {
        Vec4 unitVec = *this;
        unitVec.NormalizeUnsafe();
        return unitVec;
    }


    //-----------------------------------------------------------
    //! @brief              方向ベクトルとベクトルの長さの取得
    //! 
    //! @param[out] outDir  方向ベクトルの出力
    //! @param[out] outLen  ベクトルの大きさ
    //-----------------------------------------------------------
    inline void Vec4::ToDirectionAndLength(Vec4& outDir, f32& outLen)const
    {
        if (IsZero())
        {
            outDir.SetZero();
            outLen = 0.0f;
            return;
        }
        outLen = SqrMag();
        outDir = (*this) / outLen;
    }


    //===========================================================
    //  変換
    //===========================================================

    //-----------------------------------------------------------
    //! @brief              ベクトルの正規化
    //! 
    //! @details            0ベクトルの場合は何もしない。
    //! @param tolerance    ゼロベクトル判定の誤差の許容範囲
    //-----------------------------------------------------------
    inline void Vec4::Normalize(f32 tolerance)
    {
        f32 f = Mag();
        // 0ベクトルの場合は何もしない
        if (f <= tolerance)return;
        this->operator/=(f);
    }


    //-----------------------------------------------------------
    //! @brief              ベクトルの正規化(ゼロチェックなし)
    //-----------------------------------------------------------
    inline void Vec4::NormalizeUnsafe()
    {
        f32 f = Mag();
        this->operator/=(f);
    }


    //===============================================================
    //  判定
    //===============================================================

    //-----------------------------------------------------------
    //! @brief 単位ベクトルか判定
    //-----------------------------------------------------------
    inline b32 Vec4::IsUnit(f32 tolerance)const
    {
        const f32 mag = SqrMag();
        return Mathf::Abs(mag - 1.0f) < tolerance;
    }


    //-----------------------------------------------------------
    //! @brief 等価判定(許容範囲指定)
    //! 
    //! @param tolerance    許容範囲
    //-----------------------------------------------------------
    inline b32 Vec4::Equals(const Vec4& v, f32 tolerance)const
    {
        return
            Mathf::Abs(x - v.x) <= tolerance &&
            Mathf::Abs(y - v.y) <= tolerance &&
            Mathf::Abs(z - v.z) <= tolerance &&
            Mathf::Abs(w - v.w) <= tolerance;
    }


    //-----------------------------------------------------------
    //! @brief すべての要素が同じ値か判定
    //! 
    //! @param tolerance    許容範囲
    //-----------------------------------------------------------
    inline b32 Vec4::AllComponentsEqual(f32 tolerance)const
    {
        if (tolerance < Mathf::Abs(x - y))return FALSE;
        if (tolerance < Mathf::Abs(y - z))return FALSE;
        if (tolerance < Mathf::Abs(z - w))return FALSE;
        if (tolerance < Mathf::Abs(w - x))return FALSE;
        if (tolerance < Mathf::Abs(x - z))return FALSE;
        if (tolerance < Mathf::Abs(y - w))return FALSE;
        return TRUE;
    }


    //-----------------------------------------------------------
    //! @brief ゼロベクトルか判定
    //-----------------------------------------------------------
    inline b32 Vec4::IsZero(f32 tolerance)const
    {
        if (tolerance < Mathf::Abs(x))return FALSE;
        if (tolerance < Mathf::Abs(y))return FALSE;
        if (tolerance < Mathf::Abs(z))return FALSE;
        if (tolerance < Mathf::Abs(w))return FALSE;
        return TRUE;
    }


    //-----------------------------------------------------------
    //! @brief 要素にNaNを含むか判定
    //-----------------------------------------------------------
    inline b32 Vec4::IsNaN()const
    {
        if (Mathf::IsNaN(x))return TRUE;
        if (Mathf::IsNaN(y))return TRUE;
        if (Mathf::IsNaN(z))return TRUE;
        if (Mathf::IsNaN(w))return TRUE;
        return FALSE;
    }


    //===============================================================
    //  スタティック関数
    //===============================================================

    //-----------------------------------------------------------
    //! @brief ベクトルの各要素の絶対値からベクトルを作成
    //-----------------------------------------------------------
    inline Vec4 Vec4::Abs(const Vec4& v)
    {
        return Vec4(Mathf::Abs(v.x), Mathf::Abs(v.y), Mathf::Abs(v.z), Mathf::Abs(v.w));
    }


    //-----------------------------------------------------------
    //! @brief 内積
    //-----------------------------------------------------------
    inline f32 Vec4::Dot(const Vec4& a, const Vec4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }


    //-----------------------------------------------------------
    //! @brief 外積
    //-----------------------------------------------------------
    inline Vec4 Vec4::Cross(const Vec4& a, const Vec4& b)
    {
        return Vec4(a.y * b.z - a.z * b.y, a.z * b.w - a.w * b.z, a.w * b.x - a.x * b.w, a.x * b.y - a.y * b.x);
    }


    //-----------------------------------------------------------
    //! @brief  二つのベクトルの距離を計算
    //-----------------------------------------------------------
    inline f32 Vec4::Dist(const Vec4& a, const Vec4& b)
    {
        return (b - a).Mag();
    }


    //-----------------------------------------------------------
    //! @brief  二つのベクトルの距離の二乗を計算
    //-----------------------------------------------------------
    inline f32 Vec4::SqrDist(const Vec4& a, const Vec4& b)
    {
        return (b - a).SqrMag();
    }


    //-----------------------------------------------------------
    //! @brief  二つのベクトルのマンハッタン距離を計算
    //-----------------------------------------------------------
    inline f32 Vec4::MDist(const Vec4& a, const Vec4& b)
    {
        return Mathf::Abs(a.x - b.x) + Mathf::Abs(a.y - b.y) + Mathf::Abs(a.z - b.z) + Mathf::Abs(a.w - b.w);
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルの線形補間
    //! 
    //! @param a    開始ベクトル
    //! @param b    終了ベクトル
    //! @param t    補間係数
    //! @return     t=0のときa、t=1の時bを返す。
    //-----------------------------------------------------------
    inline Vec4 Vec4::Lerp(const Vec4& a, const Vec4& b, f32 t)
    {
        return Vec4(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t,
            a.w + (b.w - a.w) * t
        );
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルの球面補間
    //! 
    //! @param a    開始ベクトル
    //! @param b    終了ベクトル
    //! @param t    補間係数
    //! @return     t=0のときa、t=1の時bを返す。
    //-----------------------------------------------------------
    inline Vec4 Vec4::Slerp(const Vec4& a, const Vec4& b, f32 t)
    {
        f32 al, bl;
        Vec4 an, bn;
        a.ToDirectionAndLength(an, al);
        b.ToDirectionAndLength(bn, bl);
        f32 angle = Mathf::Acos(Dot(an, bn));
        f32 sinTh = Mathf::Sin(angle);
        f32 ps = Mathf::Sin(angle * (1.0f - t)) * Mathf::Lerp(al, bl, t);
        f32 pe = Mathf::Sin(angle * t) * Mathf::Lerp(al, bl, t);

        return Vec4(
            an.x * ps + bn.x * pe,
            an.y * ps + bn.y * pe,
            an.z * ps + bn.z * pe,
            an.w * ps + bn.w * pe
        );
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
    //-----------------------------------------------------------
    inline Vec4 Vec4::Max(const Vec4& a, const Vec4& b)
    {
        return Vec4(Mathf::Max(a.x, b.x), Mathf::Max(a.y, b.y), Mathf::Max(a.z, b.z), Mathf::Max(a.w, b.w));
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
    //-----------------------------------------------------------
    inline Vec4 Vec4::Min(const Vec4& a, const Vec4& b)
    {
        return Vec4(Mathf::Min(a.x, b.x), Mathf::Min(a.y, b.y), Mathf::Min(a.z, b.z), Mathf::Min(a.w, b.w));
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルのなす角のコサインを取得
    //-----------------------------------------------------------
    inline f32 Vec4::CosAngle(const Vec4& a, const Vec4& b)
    {
        return Dot(a.UnitVec(), b.UnitVec());
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルのなす角を取得
    //-----------------------------------------------------------
    inline f32 Vec4::Angle(const Vec4& a, const Vec4& b)
    {
        return Mathf::AcosD(CosAngle(a, b));
    }

}// namespace ob
