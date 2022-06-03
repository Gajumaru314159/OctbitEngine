﻿//***********************************************************
//! @file
//! @brief Quaternion
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Math.h>

namespace ob::core {
    struct Vec3;
    struct Rot;
    struct Matrix;

    //@―---------------------------------------------------------------------------
    //! @brief      クォータニオン(四元数)
    //! 
    //! @details    Quaternionは回転を表すのに使用される。
    //!             オイラー角と違いジンバルロックがなく、簡単に補完可能である。
    //!             Quaternionは複素数に基づいており直感的に理解することは難しい。そのため各成分(x,y,z,w)にアクセスすることは推奨しない。
    //! @note       回転へのアクセスや構築はオイラー角表現を通して行うことが推奨される。
    //@―---------------------------------------------------------------------------
    struct OB_API Quat {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Quat()noexcept = default;


        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(Identityで初期化)
        //@―---------------------------------------------------------------------------
        explicit Quat(EForceInit)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief		コンストラクタ(内部値指定)
        //! 
        //! @details	与えられた x、y、z、w 成分で新規のクォータニオンを作成します。
        //! @deprecated	成分は複素数に基づいているため、通常は使用しない関数です。
        //@―---------------------------------------------------------------------------
        Quat(f32 _x, f32 _y, f32 _z, f32 _w)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			コンストラクタ(ロール・ピッチ・ヨー)
        //! 
        //! @param roll		ロール(Degrees)
        //! @param pitch	ピッチ(Degrees)
        //! @param yaw		ヨー(Degrees)
        //@―---------------------------------------------------------------------------
        Quat(f32 roll, f32 pitch, f32 yaw);


        //@―---------------------------------------------------------------------------
        //! @brief			コンストラクタ(ロール・ピッチ・ヨー)
        //! 
        //! @param eulerAngles オイラー角(Degrees)
        //@―---------------------------------------------------------------------------
        explicit Quat(const Vec3& eulerAngles);


        //@―---------------------------------------------------------------------------
        //! @brief      乗算演算子
        //@―---------------------------------------------------------------------------
        Quat(f32 angle, const Vec3& axis);


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      乗算演算子
        //@―---------------------------------------------------------------------------
        Quat operator *  (const Quat& quat) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      乗算代入演算子
        //@―---------------------------------------------------------------------------
        Quat operator *= (const Quat& quat) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      乗算演算子
        //@―---------------------------------------------------------------------------
        Quat operator *  (f32 scalar) const;


        //@―---------------------------------------------------------------------------
        //! @brief      乗算代入演算子
        //@―---------------------------------------------------------------------------
        Quat operator *= (f32 scalar);


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子
        //@―---------------------------------------------------------------------------
        Quat operator /  (const Quat& quat) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子
        //@―---------------------------------------------------------------------------
        Quat operator /= (const Quat& quat) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 等価演算子
        //@―---------------------------------------------------------------------------
        bool operator == (const Quat& rhs)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 等価演算子
        //@―---------------------------------------------------------------------------
        bool operator != (const Quat& rhs)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief ベクトル乗算演算子(回転)
        //@―---------------------------------------------------------------------------
        Vec3 operator *  (const Vec3& v) const noexcept;


        //===============================================================
        //  設定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief ロール・ピッチ・ヨー設定
        //@―---------------------------------------------------------------------------
        void set(f32 x, f32 y, f32 z, f32 w) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief ロール・ピッチ・ヨー設定
        //@―---------------------------------------------------------------------------
        void set(f32 roll, f32 pitch, f32 yaw);


        //@―---------------------------------------------------------------------------
        //! @brief ロール・ピッチ・ヨー設定
        //@―---------------------------------------------------------------------------
        void set(const Vec3& rotation);


        //@―---------------------------------------------------------------------------
        //! @brief 任意軸回転設定
        //@―---------------------------------------------------------------------------
        void set(f32 angle, const Vec3& axis);


        //@―---------------------------------------------------------------------------
        //! @brief 回転なし状態に設定
        //@―---------------------------------------------------------------------------
        void setIdentity() noexcept;


        //===============================================================
        //  取得
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 回転後の前ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3    getFront()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転後の後ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3    getBack()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転後の上ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3    getUp()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転後の下ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3    getDown()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転後の左ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3    getLeft()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転後の右ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3    getRight()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief ベクトルの長さを取得
        //! 
        //! @details 二つの Quat の長さを比較する場合はSqrMagのほうが高速です。
        //@―---------------------------------------------------------------------------
        f32     getMag() const;


        //@―---------------------------------------------------------------------------
        //! @brief ベクトルの長さの二乗を取得
        //@―---------------------------------------------------------------------------
        f32     getSqrMag() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 正規化
        //! 
        //! @note 計算誤差によりノルムが1でなくなった場合は明示的に正規化する必要がある
        //@―---------------------------------------------------------------------------
        Quat    getNorm()const;


        //@―---------------------------------------------------------------------------
        //! @brief 行列表現を取得
        //@―---------------------------------------------------------------------------
        Matrix  getMatrix() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief オイラー角での表現を取得
        //@―---------------------------------------------------------------------------
        Rot getRot() const;


        //@―---------------------------------------------------------------------------
        //! @brief 回転軸を取得
        //! 
        //! @return 回転量が0の場合はVector3(0.0f,1.0f,0.0f)を返す。
        //@―---------------------------------------------------------------------------
        Vec3    getAxis() const;


        //@―---------------------------------------------------------------------------
        //! @brief 回転量を取得
        //@―---------------------------------------------------------------------------
        f32     getAngle() const;


        //@―---------------------------------------------------------------------------
        //! @brief 逆回転を表すQuaternionを取得
        //@―---------------------------------------------------------------------------
        Quat    getInverse() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転軸と回転量を取得
        //@―---------------------------------------------------------------------------
        void    toAxisAndAngle(f32& angle, Vec3& axis) const;


        //@―---------------------------------------------------------------------------
        //! @brief 回転軸と回転量を取得
        //@―---------------------------------------------------------------------------
        void    toSwingTwist(const Vec3& twistAxis, Quat& outSwing, Quat& outTwist) const;


        //===============================================================
        //  変換
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief グローバル回転
        //@―---------------------------------------------------------------------------
        void rotate(const Vec3& eulerAngles);


        //@―---------------------------------------------------------------------------
        //! @brief グローバル回転
        //@―---------------------------------------------------------------------------
        void rotate(f32 x, f32 y, f32 z);


        //@―---------------------------------------------------------------------------
        //! @brief ローカル回転
        //@―---------------------------------------------------------------------------
        void rotateLocal(const Vec3& eulerAngles);


        //@―---------------------------------------------------------------------------
        //! @brief ローカル回転
        //@―---------------------------------------------------------------------------
        void rotateLocal(f32 x, f32 y, f32 z);


        //@―---------------------------------------------------------------------------
        //! @brief グローバルX軸回転
        //@―---------------------------------------------------------------------------
        void rotateX(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief グローバルY軸回転
        //@―---------------------------------------------------------------------------
        void rotateY(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief グローバルZ軸回転
        //@―---------------------------------------------------------------------------
        void rotateZ(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief ローカルX軸回転
        //@―---------------------------------------------------------------------------
        void rotateLocalX(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief ローカルY軸回転
        //@―---------------------------------------------------------------------------
        void rotateLocalY(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief ローカルZ軸回転
        //@―---------------------------------------------------------------------------
        void rotateLocalZ(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief 回転を逆回転にする
        //@―---------------------------------------------------------------------------
        void invert() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 正規化
        //! 
        //! @note 計算誤差によりノルムが1でなくなった場合は明示的に正規化する必要がある
        //@―---------------------------------------------------------------------------
        void normalize();


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 等価判定(許容誤差指定)
        //! 
        //! @param v            比較対象
        //! @param tolerance    許容誤差
        //@―---------------------------------------------------------------------------
        bool equals(const Quat& v, f32 tolerance = Math::TOLERANCE)const noexcept;         // 等価判定(許容誤差指定)


    public:

        //@―---------------------------------------------------------------------------
        //! @brief 方向ベクトルをfromDirからtoDirに回転するQuaternionの計算
        //@―---------------------------------------------------------------------------
        static Quat FromTo(const Vec3& fromDir, const Vec3& toDir);


        //@―---------------------------------------------------------------------------
        //! @brief ターゲット方向に向けるQuaternionを計算
        //@―---------------------------------------------------------------------------
        static Quat LookAt(const Vec3& target, const Vec3& up);


        //@―---------------------------------------------------------------------------
        //! @brief Quaternionの線形補完
        //@―---------------------------------------------------------------------------
        static Quat Lerp(const Quat& a, const Quat& b, f32 t);


        //@―---------------------------------------------------------------------------
        //! @brief Quaternionの球面補完
        //@―---------------------------------------------------------------------------
        static Quat Slerp(const Quat& a, const Quat& b, f32 t);


        //@―---------------------------------------------------------------------------
        //! @brief 内積
        //@―---------------------------------------------------------------------------
        static f32 Dot(const Quat& a, const Quat& b) noexcept;

    public:

        static const Quat identity;     //!< 初期状態

    public:

        f32 x;//!< X成分
        f32 y;//!< Y成分
        f32 z;//!< Z成分
        f32 w;//!< W成分

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(Identityで初期化)
    //@―---------------------------------------------------------------------------
    inline Quat::Quat(EForceInit)noexcept {
        setIdentity();
    }


    //@―---------------------------------------------------------------------------
    //! @brief		コンストラクタ(内部値指定)
    //! 
    //! @details	与えられた x、y、z、w 成分で新規のクォータニオンを作成します。
    //! @deprecated	成分は複素数に基づいているため、通常は使用しない関数です。
    //@―---------------------------------------------------------------------------
    inline Quat::Quat(f32 _x, f32 _y, f32 _z, f32 _w)  noexcept
        :x(_x), y(_y), z(_z), w(_w) {
    };


    //@―---------------------------------------------------------------------------
    //! @brief			コンストラクタ(ロール・ピッチ・ヨー)
    //! 
    //! @param roll		ロール
    //! @param pitch	ピッチ
    //! @param yaw		ヨー
    //@―---------------------------------------------------------------------------
    inline Quat::Quat(f32 roll, f32 pitch, f32 yaw) {
        set(roll, pitch, yaw);
    }


    //@―---------------------------------------------------------------------------
    //! @brief			コンストラクタ(任意軸回転)
    //! 
    //! @param rotation オイラー角
    //@―---------------------------------------------------------------------------
    inline Quat::Quat(f32 angle, const Vec3& axis) {
        set(angle, axis);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      乗算演算子
    //@―---------------------------------------------------------------------------
    inline Quat Quat::operator * (const Quat& quat) const noexcept {
        return Quat(*this) *= quat;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      乗算代入演算子
    //@―---------------------------------------------------------------------------
    inline Quat Quat::operator *= (const Quat& quat) noexcept {
        f32 x = this->x;
        f32 y = this->y;
        f32 z = this->z;
        f32 w = this->w;

        this->x = y * quat.z - z * quat.y + x * quat.w + w * quat.x;
        this->y = z * quat.x - x * quat.z + y * quat.w + w * quat.y;
        this->z = x * quat.y - y * quat.x + z * quat.w + w * quat.z;
        this->w = w * quat.w - x * quat.x - y * quat.y - z * quat.z;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子
    //@―---------------------------------------------------------------------------
    inline Quat Quat::operator / (const Quat& quat) const noexcept {
        return (*this) * quat.getInverse();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子
    //@―---------------------------------------------------------------------------
    inline Quat Quat::operator /= (const Quat& quat) noexcept {
        (*this) *= quat.getInverse();
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Quat::operator == (const Quat& quat)const noexcept {
        return
            Math::IsNearEquals(x, quat.x) &&
            Math::IsNearEquals(y, quat.y) &&
            Math::IsNearEquals(z, quat.z) &&
            Math::IsNearEquals(w, quat.w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Quat::operator != (const Quat& quat)const noexcept {
        return !(operator==(quat));
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバル回転
    //@―---------------------------------------------------------------------------
    inline void Quat::rotate(f32 x, f32 y, f32 z) {
        (*this) = Quat(x, y, z) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカル回転
    //@―---------------------------------------------------------------------------
    inline void Quat::rotateLocal(f32 x, f32 y, f32 z) {
        (*this) *= Quat(x, y, z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバルX軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::rotateX(f32 degrees) {
        (*this) = Quat(degrees, 0, 0) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバルY軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::rotateY(f32 degrees) {
        (*this) = Quat(0, degrees, 0) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバルZ軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::rotateZ(f32 degrees) {
        (*this) = Quat(0, 0, degrees) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカルX軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::rotateLocalX(f32 degrees) {
        (*this) *= Quat(degrees, 0, 0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカルY軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::rotateLocalY(f32 degrees) {
        (*this) *= Quat(0, degrees, 0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカルZ軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::rotateLocalZ(f32 degrees) {
        (*this) *= Quat(0, 0, degrees);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 逆回転を表すQuaternionを取得
    //@―---------------------------------------------------------------------------
    inline Quat Quat::getInverse() const noexcept {
        return Quat(-x, -y, -z, w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ロール・ピッチ・ヨー設定
    //@―---------------------------------------------------------------------------
    inline void Quat::set(f32 x_, f32 y_, f32 z_, f32 w_) noexcept {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ロール・ピッチ・ヨー設定
    //@―---------------------------------------------------------------------------
    inline void Quat::set(f32 roll, f32 pitch, f32 yaw) {
        f32 sx = Math::Sin(Math::Degrees(roll * 0.5f));
        f32 cx = Math::Cos(Math::Degrees(roll * 0.5f));
        f32 sy = Math::Sin(Math::Degrees(pitch * 0.5f));
        f32 cy = Math::Cos(Math::Degrees(pitch * 0.5f));
        f32 sz = Math::Sin(Math::Degrees(yaw * 0.5f));
        f32 cz = Math::Cos(Math::Degrees(yaw * 0.5f));
        //this->x = cy * sx * cz - sy * cx * sz;
        //this->y = cy * sx * sz + sy * cx * sz;
        //this->z = cy * cx * sz + sy * sx * cz;
        //this->w = cy * cx * cz - sy * sx * sz;
        *this = Quat(0, sy, 0, cy) * Quat(sx, 0, 0, cx) * Quat(0, 0, sz, cz);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転なし状態に設定
    //@―---------------------------------------------------------------------------
    inline void Quat::setIdentity() noexcept {
        set(0.0f, 0.0f, 0.0f, 1.0f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの長さを取得
    //! 
    //! @details 二つの Quat の長さを比較する場合はSqrMagのほうが高速です。
    //@―---------------------------------------------------------------------------
    inline f32 Quat::getMag() const {
        return Math::Sqrt(getSqrMag());
    }

    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの長さの二乗を取得
    //@―---------------------------------------------------------------------------
    inline f32 Quat::getSqrMag() const noexcept {
        return x * x + y * y + z * z + w * w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 正規化
    //! 
    //! @note 計算誤差によりノルムが1でなくなった場合は明示的に正規化する必要がある
    //@―---------------------------------------------------------------------------
    inline void Quat::normalize() {
        f32 div = 1.0f / Math::Sqrt(x * x + y * y + z * z + w * w);
        x *= div;
        y *= div;
        z *= div;
        w *= div;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 正規化
    //! 
    //! @note 計算誤差によりノルムが1でなくなった場合は明示的に正規化する必要がある
    //@―---------------------------------------------------------------------------
    inline Quat Quat::getNorm()const {
        f32 div = 1.0f / Math::Sqrt(x * x + y * y + z * z + w * w);
        return Quat(x * div, y * div, z * div, w * div);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転量を取得
    //@―---------------------------------------------------------------------------
    inline f32 Quat::getAngle() const {
        return Math::Radians(Math::Acos(w) * 2.0f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 内積
    //@―---------------------------------------------------------------------------
    inline f32 Quat::Dot(const Quat& a, const Quat& b) noexcept {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価判定(許容誤差指定)
    //! 
    //! @param tolerance    許容誤差
    //@―---------------------------------------------------------------------------
    inline bool Quat::equals(const Quat& v, f32 tolerance)const noexcept {
        return
            Math::Abs(x - v.x) <= tolerance &&
            Math::Abs(y - v.y) <= tolerance &&
            Math::Abs(z - v.z) <= tolerance &&
            Math::Abs(w - v.w) <= tolerance;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転を逆回転にする
    //@―---------------------------------------------------------------------------
    inline void Quat::invert() noexcept {
        x *= -1.0f;
        y *= -1.0f;
        z *= -1.0f;
    }


    //! @endcond
}