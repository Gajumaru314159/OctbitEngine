//***********************************************************
//! @file
//! @brief 三次元ベクトル
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Math/Mathf.hpp>

namespace ob
{

    //-----------------------------------------------------------
    //! @brief 三次元ベクトル
    //-----------------------------------------------------------
    struct OB_API Vec3
    {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        Vec3();                                             // コンストラクタ(ゼロ初期化)
        explicit Vec3(f32 scalar);                          // コンストラクタ(すべての要素を同じ値で初期化)
        Vec3(f32 x, f32 y, f32 z);                          // コンストラクタ(要素を指定して初期化)


        //===============================================================
        //  オペレータ
        //===============================================================
        bool operator == (const Vec3& v) const;             // 等価演算子
        bool operator != (const Vec3& v) const;             // 否等価演算子

        Vec3 operator + (const Vec3& v) const;              // 加算演算子
        Vec3 operator + (f32 bias) const;                   // 加算演算子(スカラー)
        Vec3 operator - (const Vec3& v) const;              // 減算演算子
        Vec3 operator - (f32 bias) const;                   // 減算演算子(スカラー)
        Vec3 operator * (const Vec3& v) const;              // 乗算演算子
        Vec3 operator * (f32 scale) const;                  // 乗算演算子(スカラー)
        friend Vec3 operator * (f32 f, const Vec3& v);      // 乗算演算子(前方スカラー)
        Vec3 operator / (const Vec3& v) const;              // 除算演算子
        Vec3 operator / (f32 scale) const;                  // 除算演算子(スカラー)
        friend Vec3 operator / (f32 f, const Vec3& v);      // 除算演算子(前方スカラー)

        Vec3& operator += (const Vec3& v);                  // 加算代入演算子
        Vec3& operator += (f32 bias);                       // 加算代入演算子(スカラー)
        Vec3& operator -= (const Vec3& v);                  // 減算代入演算子
        Vec3& operator -= (f32 bias);                       // 減算代入演算子(スカラー)
        Vec3& operator *= (const Vec3& v);                  // 乗算代入演算子
        Vec3& operator *= (f32 scale);                      // 乗算代入演算子(スカラー)
        Vec3& operator /= (const Vec3& v);                  // 除算代入演算子
        Vec3& operator /= (f32 scale);                      // 除算代入演算子(スカラー)

        Vec3 operator - () const;                           // 単項マイナス演算子


        //===============================================================
        //  セッター
        //===============================================================;
        void    Set(f32 x, f32 y, f32 z);                   // 要素の設定
        void    Set(const Vec3& v);                         // 要素の設定
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
        Vec3    UnitVec(f32 tolerance = Mathf::TOLERANCE)const;                 // 単位ベクトルの取得
        Vec3    UnitVecUnsafe()const;                                           // 単位ベクトルの取得(ゼロチェックなし)
        void    ToDirectionAndLength(Vec3& outDir, f32& outLen)const;           // 方向ベクトルとベクトルの長さの取得


        //===========================================================
        //  変換
        //===========================================================
        void    Normalize(f32 tolerance = Mathf::TOLERANCE);                    // ベクトルの正規化
        void    NormalizeUnsafe();                                              // ベクトルの正規化(ゼロチェックなし)
        void    Translate(f32 x, f32 y, f32 z);                                 // 移動
        void    Translate(const Vec3& add);                                     // 移動
        void    Rotate(const Vec3& rot);                                        // 回転
        void    Rotate(f32 x, f32 y, f32 z);                                    // 回転
        void    Rotate(f32 angle, const Vec3& axis);                            // 任意軸回転
        void    RotateX(f32 theta);                                             // X軸回転
        void    RotateY(f32 theta);                                             // Y軸回転
        void    RotateZ(f32 theta);                                             // Z軸回転
        void    Scale(f32 scale);                                               // 拡大
        void    Scale(f32 sx, f32 sy, f32 sz);                                  // 拡大
        void    Scale(const Vec3& scale);                                       // 拡大

        //===============================================================
        //  判定
        //===============================================================
        b32     IsUnit(f32 tolerance = Mathf::TOLERANCE)const;                  // 単位ベクトルか判定
        b32     Equals(const Vec3& v, f32 tolerance = Mathf::TOLERANCE)const;   // 等価判定(許容範囲指定)
        b32     AllComponentsEqual(f32 tolerance = Mathf::TOLERANCE)const;      // すべての要素が同じ値か判定
        b32     IsZero(f32 tolerance = Mathf::TOLERANCE)const;                  // ゼロベクトルか判定
        b32     IsNaN()const;                                                   // 要素にNaNを含むか判定

    public:

        //===============================================================
        //  スタティック関数
        //===============================================================
        static Vec3 Abs(const Vec3& v);                                         // ベクトルの各要素の絶対値からベクトルを作成
        static f32  Dot(const Vec3& a, const Vec3& b);                          // 内積
        static Vec3 Cross(const Vec3& a, const Vec3& b);                        // 外積
        static f32  Dist(const Vec3& a, const Vec3& b);                         // 二つのベクトルの距離を計算
        static f32  SqrDist(const Vec3& a, const Vec3& b);                      // 二つのベクトルの距離の二乗を計算
        static f32  DistXY(const Vec3& a, const Vec3& b);                       // 二つのベクトルのXY平面上の距離を計算
        static f32  SqrDistXY(const Vec3& a, const Vec3& b);                    // 二つのベクトルのXY平面上の距離の二乗を計算
        static f32  DistYZ(const Vec3& a, const Vec3& b);                       // 二つのベクトルのYZ平面上の距離を計算
        static f32  SqrDistYZ(const Vec3& a, const Vec3& b);                    // 二つのベクトルのYZ平面上の距離の二乗を計算
        static f32  DistXZ(const Vec3& a, const Vec3& b);                       // 二つのベクトルのXZ平面上の距離を計算
        static f32  SqrDistXZ(const Vec3& a, const Vec3& b);                    // 二つのベクトルのXZ平面上の距離の二乗を計算
        static f32  MDist(const Vec3& a, const Vec3& b);                        // 二つのベクトルのマンハッタン距離を計算
        static Vec3 Lerp(const Vec3& a, const Vec3& b, f32 t);                  // 二つのベクトルの線形補間
        static Vec3 Slerp(const Vec3& a, const Vec3& b, f32 t);                 // 二つのベクトルの球面補間
        static Vec3 Max(const Vec3& a, const Vec3& b);                          // 二つのベクトルの各要素の大きいほうからベクトルを生成
        static Vec3 Min(const Vec3& a, const Vec3& b);                          // 二つのベクトルの各要素の小さいほうからベクトルを生成
        static f32  CosAngle(const Vec3& a, const Vec3& b);                     // 二つのベクトルのなす角のコサインを計算
        static f32  Angle(const Vec3& a, const Vec3& b);                        // 二つのベクトルのなす角を計算
        static Vec3 Mirror(const Vec3& inDir, const Vec3& normal);              // 反射ベクトルの計算
        static Vec3 Projection(const Vec3& inDir, const Vec3& normal);          // 投影ベクトルの計算

    public:

        static const Vec3 up;      //!< ( 0.0f, 1.0f, 0.0f)
        static const Vec3 down;    //!< ( 0.0f,-1.0f, 0.0f)
        static const Vec3 left;    //!< (-1.0f, 0.0f, 0.0f)
        static const Vec3 right;   //!< ( 1.0f, 0.0f, 0.0f)
        static const Vec3 front;   //!< ( 0.0f, 0.0f, 1.0f)
        static const Vec3 back;    //!< ( 0.0f, 0.0f,-1.0f)
        static const Vec3 zero;    //!< ( 0.0f, 0.0f, 0.0f)
        static const Vec3 one;     //!< ( 1.0f, 1.0f, 1.0f)

    public:

        union
        {
            struct
            {
                f32 x;    //!< ベクトルのX成分
                f32 y;    //!< ベクトルのY成分
                f32 z;    //!< ベクトルのZ成分
            };
            f32 data[3];  //!< ベクトルの成分
        };

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------
    //! @brief      コンストラクタ(ゼロ初期化)
    //-----------------------------------------------------------
    inline Vec3::Vec3() {
        SetZero();
    }


    //-----------------------------------------------------------
    //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
    //-----------------------------------------------------------
    inline Vec3::Vec3(f32 scalar)
    {
        Set(scalar, scalar, scalar);
    }


    //-----------------------------------------------------------
    //! @brief      コンストラクタ(要素を指定して初期化)
    //-----------------------------------------------------------
    inline Vec3::Vec3(f32 x, f32 y, f32 z)
    {
        Set(x, y, z);
    }


    //===============================================================
    //  オペレータ
    //===============================================================


    //-----------------------------------------------------------
    //! @brief      等価演算子 
    //! 
    //! @details    2つのベクトルの差が 1e-5 より小さい場合、2つのベクトルは等しいと見なされます。
    //-----------------------------------------------------------
    inline bool Vec3::operator == (const Vec3& v) const
    {
        return Mathf::Approximately(x, v.x) &&
            Mathf::Approximately(y, v.y) &&
            Mathf::Approximately(z, v.z);
    }


    //-----------------------------------------------------------
    //! @brief 否等価演算子
    //! 
    //! @details    2つのベクトルの差が 1e-5 より大きい場合、2つのベクトルは異なると見なされます。
    //-----------------------------------------------------------
    inline bool Vec3::operator != (const Vec3& v) const
    {
        return !(operator==(v));
    }


    //-----------------------------------------------------------
    //! @brief 加算演算子
    //-----------------------------------------------------------
    inline Vec3 Vec3::operator + (const Vec3& v) const
    {
        return Vec3(*this) += v;
    }


    //-----------------------------------------------------------
    //! @brief 加算演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec3 Vec3::operator + (f32 scalar) const
    {
        return Vec3(*this) += scalar;
    }


    //-----------------------------------------------------------
    //! @brief 減算演算子
    //-----------------------------------------------------------
    inline Vec3 Vec3::operator - (const Vec3& v) const
    {
        return Vec3(*this) -= v;
    }


    //-----------------------------------------------------------
    //! @brief 減算演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec3 Vec3::operator - (f32 scalar) const
    {
        return Vec3(*this) -= scalar;
    }


    //-----------------------------------------------------------
    //! @brief 乗算演算子
    //-----------------------------------------------------------
    inline Vec3 Vec3::operator * (const Vec3& v) const
    {
        return Vec3(*this) *= v;
    }


    //-----------------------------------------------------------
    //! @brief 乗算演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec3 Vec3::operator * (f32 scalar) const
    {
        return Vec3(*this) *= scalar;
    }


    //-----------------------------------------------------------
    //! @brief 除算演算子
    //-----------------------------------------------------------
    inline Vec3 Vec3::operator / (const Vec3& v) const
    {
        return Vec3(*this) /= v;
    }


    //-----------------------------------------------------------
    //! @brief 除算演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec3 Vec3::operator / (f32 scalar) const
    {
        return Vec3(*this) /= scalar;
    }


    //-----------------------------------------------------------
    //! @brief 加算代入演算子
    //-----------------------------------------------------------
    inline Vec3& Vec3::operator += (const Vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 加算代入演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec3& Vec3::operator += (f32 scalar)
    {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 減算代入演算子 
    //-----------------------------------------------------------
    inline Vec3& Vec3::operator -= (const Vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 減算代入演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec3& Vec3::operator -= (f32 scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 乗算代入演算子 
    //-----------------------------------------------------------
    inline Vec3& Vec3::operator *= (const Vec3& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 乗算代入演算子 (スカラー)
    //-----------------------------------------------------------
    inline Vec3& Vec3::operator *= (f32 scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 除算代入演算子 
    //-----------------------------------------------------------
    inline Vec3& Vec3::operator /= (const Vec3& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 除算代入演算子(スカラー)
    //-----------------------------------------------------------
    inline Vec3& Vec3::operator /= (f32 scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }


    //-----------------------------------------------------------
    //! @brief 単項マイナス演算子
    //-----------------------------------------------------------
    inline Vec3 Vec3::operator - () const
    {
        return Vec3(-x, -y, -z);
    }


    //===============================================================
    //  セッター
    //===============================================================

    //-----------------------------------------------------------
    //! @brief 要素の設定
    //-----------------------------------------------------------
    inline void Vec3::Set(f32 x, f32 y, f32 z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }


    //-----------------------------------------------------------
    //! @brief 要素の設定
    //-----------------------------------------------------------
    inline void Vec3::Set(const Vec3& v)
    {
        Set(v.x, v.y, v.z);
    }


    //-----------------------------------------------------------
    //! @brief 要素を全て0に設定
    //-----------------------------------------------------------
    inline void Vec3::SetZero()
    {
        SetAll(0.0f);
    }


    //-----------------------------------------------------------
    //! @brief 全ての要素を同じ値で設定
    //-----------------------------------------------------------
    inline void Vec3::SetAll(f32 scalar)
    {
        Set(scalar, scalar, scalar);
    }


    //===============================================================
    //  ゲッター
    //===============================================================

    //-----------------------------------------------------------
    //! @brief 要素の最大を取得
    //-----------------------------------------------------------
    inline f32 Vec3::MaxComponent() const
    {
        return Mathf::Max(x, y, z);
    }


    //-----------------------------------------------------------
    //! @brief 要素の絶対値の最大を取得
    //-----------------------------------------------------------
    inline f32 Vec3::MaxAbsComponent() const
    {
        return Mathf::Max(Mathf::Abs(x), Mathf::Abs(y), Mathf::Abs(z));
    }


    //-----------------------------------------------------------
    //! @brief 要素の最小を取得
    //-----------------------------------------------------------
    inline f32 Vec3::MinComponent() const
    {
        return Mathf::Min(x, y, z);
    }


    //-----------------------------------------------------------
    //! @brief 要素の絶対値の最小を取得
    //-----------------------------------------------------------
    inline f32 Vec3::MinAbsComponent() const
    {
        return Mathf::Min(Mathf::Abs(x), Mathf::Abs(y), Mathf::Abs(z));
    }


    //-----------------------------------------------------------
    //! @brief ベクトルの長さの取得
    //! 
    //! @details 二つのベクトルの長さを比較する場合はSqrMagのほうが高速です。
    //-----------------------------------------------------------
    inline f32 Vec3::Mag() const
    {
        return Mathf::Sqrt(SqrMag());
    }


    //-----------------------------------------------------------
    //! @brief ベクトルの長さの二乗を取得
    //-----------------------------------------------------------
    inline f32 Vec3::SqrMag() const
    {
        return x * x + y * y + z * z;
    }


    //-----------------------------------------------------------
    //! @brief              単位ベクトルの取得
    //! 
    //! @details            0ベクトルの場合は0ベクトルを返す。
    //! @param tolerance    ゼロベクトル判定の誤差の許容範囲
    //-----------------------------------------------------------
    inline Vec3 Vec3::UnitVec(f32 tolerance)const
    {
        Vec3 unitVec = *this;
        unitVec.Normalize(tolerance);
        return unitVec;
    }


    //-----------------------------------------------------------
    //! @brief              単位ベクトルの取得(ゼロチェックなし)
    //-----------------------------------------------------------
    inline Vec3 Vec3::UnitVecUnsafe()const
    {
        Vec3 unitVec = *this;
        unitVec.NormalizeUnsafe();
        return unitVec;
    }


    //-----------------------------------------------------------
    //! @brief              方向ベクトルとベクトルの長さの取得
    //! 
    //! @param[out] outDir  方向ベクトルの出力
    //! @param[out] outLen  ベクトルの大きさ
    //-----------------------------------------------------------
    inline void Vec3::ToDirectionAndLength(Vec3& outDir, f32& outLen)const
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
    inline void Vec3::Normalize(f32 tolerance)
    {
        f32 f = Mag();
        // 0ベクトルの場合は何もしない
        if (f <= tolerance)return;
        this->operator/=(f);
    }


    //-----------------------------------------------------------
    //! @brief              ベクトルの正規化(ゼロチェックなし)
    //-----------------------------------------------------------
    inline void Vec3::NormalizeUnsafe()
    {
        f32 f = Mag();
        this->operator/=(f);
    }


    //-----------------------------------------------------------
    //! @brief      移動
    //-----------------------------------------------------------
    inline void Vec3::Translate(f32 x, f32 y, f32 z)
    {
        this->x += x;
        this->y += y;
        this->z += z;
    }


    //-----------------------------------------------------------
    //! @brief      移動
    //-----------------------------------------------------------
    inline void Vec3::Translate(const Vec3& v)
    {
        Translate(v.x, v.y, v.z);
    }


    //-----------------------------------------------------------
    //! @brief      回転
    //! 
    //! @details    回転順序はZXYです。
    //-----------------------------------------------------------
    inline void Vec3::Rotate(const Vec3& rot)
    {
        RotateZ(rot.z);
        RotateX(rot.x);
        RotateY(rot.y);
    }


    //-----------------------------------------------------------
    //! @brief      回転
    //! 
    //! @details    回転順序はZXYです。
    //-----------------------------------------------------------
    inline void Vec3::Rotate(f32 x, f32 y, f32 z)
    {
        RotateZ(z);
        RotateX(x);
        RotateY(y);
    }


    //-----------------------------------------------------------
    //! @brief      X軸回転
    //-----------------------------------------------------------
    inline void Vec3::RotateX(f32 theta)
    {
        const f32 lx = y;
        const f32 ly = z;
        const f32 rad = Mathf::Degrees(theta);
        const f32 sin = Mathf::Sin(rad);
        const f32 cos = Mathf::Cos(rad);
        y = lx * cos - ly * sin;
        z = lx * sin + ly * cos;
    }


    //-----------------------------------------------------------
    //! @brief      Y軸回転
    //-----------------------------------------------------------
    inline void Vec3::RotateY(f32 theta)
    {
        const f32 lx = z;
        const f32 ly = x;
        const f32 rad = Mathf::Degrees(theta);
        const f32 sin = Mathf::Sin(rad);
        const f32 cos = Mathf::Cos(rad);
        z = lx * cos - ly * sin;
        x = lx * sin + ly * cos;
    }


    //-----------------------------------------------------------
    //! @brief      Z軸回転
    //-----------------------------------------------------------
    inline void Vec3::RotateZ(f32 theta)
    {
        const f32 lx = x;
        const f32 ly = y;
        const f32 rad = Mathf::Degrees(theta);
        const f32 sin = Mathf::Sin(rad);
        const f32 cos = Mathf::Cos(rad);
        x = lx * cos - ly * sin;
        y = lx * sin + ly * cos;
    }


    //-----------------------------------------------------------
    //! @brief      拡大
    //-----------------------------------------------------------
    inline void Vec3::Scale(f32 scale)
    {
        Scale(scale, scale, scale);
    }


    //-----------------------------------------------------------
    //! @brief      拡大
    //-----------------------------------------------------------
    inline void Vec3::Scale(f32 sx, f32 sy, f32 sz)
    {
        x *= sx;
        y *= sy;
        z *= sz;
    }


    //-----------------------------------------------------------
    //! @brief      拡大
    //-----------------------------------------------------------
    inline void Vec3::Scale(const Vec3& scale)
    {
        Scale(scale.x, scale.y, scale.z);
    }


    //===============================================================
    //  判定
    //===============================================================

    //-----------------------------------------------------------
    //! @brief 単位ベクトルか判定
    //-----------------------------------------------------------
    inline b32 Vec3::IsUnit(f32 tolerance)const
    {
        const f32 mag = SqrMag();
        return Mathf::Abs(mag - 1.0f) < tolerance;
    }


    //-----------------------------------------------------------
    //! @brief 等価判定(許容範囲指定)
    //! 
    //! @param tolerance    許容範囲
    //-----------------------------------------------------------
    inline b32 Vec3::Equals(const Vec3& v, f32 tolerance)const
    {
        return
            Mathf::Abs(x - v.x) <= tolerance &&
            Mathf::Abs(y - v.y) <= tolerance &&
            Mathf::Abs(z - v.z) <= tolerance;
    }


    //-----------------------------------------------------------
    //! @brief すべての要素が同じ値か判定
    //! 
    //! @param tolerance    許容範囲
    //-----------------------------------------------------------
    inline b32 Vec3::AllComponentsEqual(f32 tolerance)const
    {
        if (tolerance < Mathf::Abs(x - y))return FALSE;
        if (tolerance < Mathf::Abs(y - z))return FALSE;
        if (tolerance < Mathf::Abs(z - x))return FALSE;
        return TRUE;
    }


    //-----------------------------------------------------------
    //! @brief ゼロベクトルか判定
    //-----------------------------------------------------------
    inline b32 Vec3::IsZero(f32 tolerance)const
    {
        if (tolerance < Mathf::Abs(x))return FALSE;
        if (tolerance < Mathf::Abs(y))return FALSE;
        if (tolerance < Mathf::Abs(z))return FALSE;
        return TRUE;
    }


    //-----------------------------------------------------------
    //! @brief 要素にNaNを含むか判定
    //-----------------------------------------------------------
    inline b32 Vec3::IsNaN()const
    {
        if (Mathf::IsNaN(x))return TRUE;
        if (Mathf::IsNaN(y))return TRUE;
        if (Mathf::IsNaN(z))return TRUE;
        return FALSE;
    }


    //===============================================================
    //  スタティック関数
    //===============================================================

    //-----------------------------------------------------------
    //! @brief ベクトルの各要素の絶対値からベクトルを作成
    //-----------------------------------------------------------
    inline Vec3 Vec3::Abs(const Vec3& v)
    {
        return Vec3(Mathf::Abs(v.x), Mathf::Abs(v.y), Mathf::Abs(v.z));
    }


    //-----------------------------------------------------------
    //! @brief 内積
    //-----------------------------------------------------------
    inline f32 Vec3::Dot(const Vec3& a, const Vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }


    //-----------------------------------------------------------
    //! @brief 外積
    //-----------------------------------------------------------
    inline Vec3 Vec3::Cross(const Vec3& a, const Vec3& b)
    {
        return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }


    //-----------------------------------------------------------
    //! @brief  二つのベクトルの距離を計算
    //-----------------------------------------------------------
    inline f32 Vec3::Dist(const Vec3& a, const Vec3& b)
    {
        return (b - a).Mag();
    }


    //-----------------------------------------------------------
    //! @brief  二つのベクトルの距離の二乗を計算
    //-----------------------------------------------------------
    inline f32 Vec3::SqrDist(const Vec3& a, const Vec3& b)
    {
        return (b - a).SqrMag();
    }
    //-----------------------------------------------------------
    //! @brief      XY平面上の距離を計算
    //-----------------------------------------------------------
    inline f32 Vec3::DistXY(const Vec3& a, const Vec3& b)
    {
        return Mathf::Sqrt(SqrDistXY(a, b));
    }

    //-----------------------------------------------------------
    //! @brief      XY平面上の距離の二乗を計算
    //-----------------------------------------------------------
    inline f32 Vec3::SqrDistXY(const Vec3& a, const Vec3& b)
    {
        return Mathf::Square(a.x - b.x) + Mathf::Square(a.y - b.y);
    }

    //-----------------------------------------------------------
    //! @brief      XZ平面上の距離を計算
    //-----------------------------------------------------------
    inline f32 Vec3::DistXZ(const Vec3& a, const Vec3& b)
    {
        return Mathf::Sqrt(SqrDistXZ(a, b));
    }

    //-----------------------------------------------------------
    //! @brief      XZ平面上の距離の二乗を計算
    //-----------------------------------------------------------
    inline f32 Vec3::SqrDistXZ(const Vec3& a, const Vec3& b)
    {
        return Mathf::Square(a.x - b.x) + Mathf::Square(a.z - b.z);
    }

    //-----------------------------------------------------------
    //! @brief      YZ平面上の距離を計算
    //-----------------------------------------------------------
    inline f32 Vec3::DistYZ(const Vec3& a, const Vec3& b)
    {
        return Mathf::Sqrt(SqrDistYZ(a, b));
    }

    //-----------------------------------------------------------
    //! @brief      YZ平面上の距離の二乗を計算
    //-----------------------------------------------------------
    inline f32 Vec3::SqrDistYZ(const Vec3& a, const Vec3& b)
    {
        return Mathf::Square(a.y - b.y) + Mathf::Square(a.z - b.z);
    }


    //-----------------------------------------------------------
    //! @brief  二つのベクトルのマンハッタン距離を計算
    //-----------------------------------------------------------
    inline f32 Vec3::MDist(const Vec3& a, const Vec3& b)
    {
        return Mathf::Abs(a.x - b.x) + Mathf::Abs(a.y - b.y) + Mathf::Abs(a.z - b.z);
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルの線形補間
    //! 
    //! @param a    開始ベクトル
    //! @param b    終了ベクトル
    //! @param t    補間係数
    //! @return     t=0のときa、t=1の時bを返す。
    //-----------------------------------------------------------
    inline Vec3 Vec3::Lerp(const Vec3& a, const Vec3& b, f32 t)
    {
        return Vec3(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t
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
    inline Vec3 Vec3::Slerp(const Vec3& a, const Vec3& b, f32 t)
    {
        f32 al, bl;
        Vec3 an, bn;
        a.ToDirectionAndLength(an, al);
        b.ToDirectionAndLength(bn, bl);
        f32 angle = Mathf::Acos(Dot(an, bn));
        f32 sinTh = Mathf::Sin(angle);
        f32 ps = Mathf::Sin(angle * (1.0f - t)) * Mathf::Lerp(al, bl, t);
        f32 pe = Mathf::Sin(angle * t) * Mathf::Lerp(al, bl, t);

        return Vec3(
            an.x * ps + bn.x * pe,
            an.y * ps + bn.y * pe,
            an.z * ps + bn.z * pe
        );
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
    //-----------------------------------------------------------
    inline Vec3 Vec3::Max(const Vec3& a, const Vec3& b)
    {
        return Vec3(Mathf::Max(a.x, b.x), Mathf::Max(a.y, b.y), Mathf::Max(a.z, b.z));
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
    //-----------------------------------------------------------
    inline Vec3 Vec3::Min(const Vec3& a, const Vec3& b)
    {
        return Vec3(Mathf::Min(a.x, b.x), Mathf::Min(a.y, b.y), Mathf::Min(a.z, b.z));
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルのなす角のコサインを取得
    //-----------------------------------------------------------
    inline f32 Vec3::CosAngle(const Vec3& a, const Vec3& b)
    {
        return Dot(a.UnitVec(), b.UnitVec());
    }


    //-----------------------------------------------------------
    //! @brief 二つのベクトルのなす角を取得
    //-----------------------------------------------------------
    inline f32 Vec3::Angle(const Vec3& a, const Vec3& b)
    {
        return Mathf::AcosD(CosAngle(a, b));
    }


    //-----------------------------------------------------------
    //! @brief      反射ベクトルの計算
    //! 
    //! @param inDir    入力ベクトル
    //! @param normal   衝突面の法線
    //-----------------------------------------------------------
    inline Vec3 Vec3::Mirror(const Vec3& inDir, const Vec3& normal)
    {
        return inDir - 2.0f * Vec3::Dot(inDir, normal) * normal;
    }


    //-----------------------------------------------------------
    //! @brief      投影ベクトルの計算
    //! 
    //! @param inDir    入力ベクトル
    //! @param normal   衝突面の法線
    //-----------------------------------------------------------
    inline Vec3 Vec3::Projection(const Vec3& inDir, const Vec3& normal)
    {
        return inDir - Vec3::Dot(inDir, normal) * normal;
    }

}// namespace ob
