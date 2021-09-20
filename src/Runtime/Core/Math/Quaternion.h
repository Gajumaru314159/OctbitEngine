//***********************************************************
//! @file
//! @brief Quaternion
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Math/Mathf.h>

namespace ob {
    struct Vec3;
    struct Matrix;

    //@―---------------------------------------------------------------------------
    //! @brief クォータニオン(四元数)
    //! 
    //! @details Quaternionは回転を表すのに使用される。
    //! オイラー角と違いジンバルロックがなく、簡単に補完可能である。
    //! Quaternionは複素数に基づいており直感的に理解することは難しい。そのため各成分(x,y,z,w)にアクセスすることは推奨しない。
    //! 回転へのアクセスや構築はオイラー角表現を通して行うことが推奨される。
    //@―---------------------------------------------------------------------------
    struct OB_API Quat {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        Quat()noexcept;                                     // デフォルトコンストラクタ
        Quat(f32 _x, f32 _y, f32 _z, f32 _w)noexcept;       // コンストラクタ(内部値指定)
        Quat(f32 roll, f32 pitch, f32 yaw);                 // コンストラクタ(ロール・ピッチ・ヨー)
        explicit Quat(const Vec3& rotation);                // コンストラクタ(ロール・ピッチ・ヨー)
        Quat(f32 angle, const Vec3& axis);                  // コンストラクタ(任意軸回転)


        //===============================================================
        //  オペレータ
        //===============================================================
        Quat operator *  (const Quat& quat) const noexcept; // 乗算演算子(回転)
        Quat operator *= (const Quat& quat) noexcept;       // 乗算代入演算子(回転)
        Quat operator *  (f32 scalar) const;                // 乗算演算子(回転)
        Quat operator *= (f32 scalar);                      // 乗算代入演算子(回転)
        Quat operator /  (const Quat& quat) const noexcept; // 除算演算子(逆回転)
        Quat operator /= (const Quat& quat) noexcept;       // 除算代入演算子(逆回転)
        bool operator == (const Quat& rhs)const noexcept;   // 等価演算子
        bool operator != (const Quat& rhs)const noexcept;   // 否等価演算子
        Vec3 operator *  (const Vec3& v) const noexcept;    // ベクトル乗算演算子(回転)


        //===============================================================
        //  回転
        //===============================================================
        void Rotate(const Vec3& rotation);                  // グローバル回転
        void Rotate(f32 x, f32 y, f32 z);                   // グローバル回転
        void RotateLocal(const Vec3& rotation);             // ローカル回転
        void RotateLocal(f32 x, f32 y, f32 z);              // ローカル回転

        void RotateX(f32 degrees);                          // グローバルX軸回転
        void RotateY(f32 degrees);                          // グローバルY軸回転
        void RotateZ(f32 degrees);                          // グローバルZ軸回転
        void RotateLocalX(f32 degrees);                     // ローカルX軸回転
        void RotateLocalY(f32 degrees);                     // ローカルY軸回転
        void RotateLocalZ(f32 degrees);                     // ローカルZ軸回転

        void Invert() noexcept;                             // 回転を逆回転にする


        //===============================================================
        //  セッター
        //===============================================================
        void Set(f32 x, f32 y, f32 z, f32 w) noexcept;      // 内部値設定
        void Set(f32 roll, f32 pitch, f32 yaw);             // ロール・ピッチ・ヨー設定
        void Set(const Vec3& rotation);                     // ロール・ピッチ・ヨー設定
        void Set(f32 angle, const Vec3& axis);              // 任意軸回転設定
        void SetIdentity() noexcept;                        // 回転なし状態に設定


        //===============================================================
        //  ゲッター
        //===============================================================
        Vec3    Front()const noexcept;                      // 回転後の前ベクトル取得
        Vec3    Back()const noexcept;                       // 回転後の後ベクトル取得
        Vec3    Up()const noexcept;                         // 回転後の上ベクトル取得
        Vec3    Down()const noexcept;                       // 回転後の下ベクトル取得
        Vec3    Left()const noexcept;                       // 回転後の左ベクトル取得
        Vec3    Right()const noexcept;                      // 回転後の右ベクトル取得

        f32     GetMag() const;                             // ベクトルの長さの取得
        f32     GetSqrMag() const noexcept;                 // ベクトルの長さの二乗を取得

        void    Normalize();                                // 正規化
        Quat    GetNorm()const;                             // ノルムの取得

        Quat    Inverse() const noexcept;                   // 逆回転を表すQuaternionを取得
        Matrix  GetMatrix() const noexcept;                 // 行列表現を取得
        Vec3    Rotation() const;                           // デグリー角での表現を取得
        Vec3    Axis() const;                               // 回転軸を取得
        f32     Angle() const;                              // 回転量を取得

        void    ToAxisAndAngle(f32& angle, Vec3& axis) const;                                // 回転軸と回転量を取得
        void    ToSwingTwist(const Vec3& twistAxis, Quat& outSwing, Quat& outTwist) const;   // 回転軸と回転量を取得


        //===============================================================
        //  判定
        //===============================================================
        bool Equals(const Quat& v, f32 tolerance = Mathf::TOLERANCE)const noexcept;         // 等価判定(許容範囲指定)

    public:

        static Quat FromTo(const Vec3& fromDir, const Vec3& toDir);         // 方向ベクトルをfromDirからtoDirに回転するQuaternionの計算
        static Quat LookAt(const Vec3& target, const Vec3& up);             // ターゲット方向に向けるQuaternionを計算

        static Quat Lerp(const Quat& a, const Quat& b, f32 t);              // 線形補間関数
        static Quat Slerp(const Quat& a, const Quat& b, f32 t);             // 球面補間関数

        static f32 Dot(const Quat& a, const Quat& b) noexcept;              // 内積

    public:

        static const Quat identity;

    public:

        f32 x;//!< X成分
        f32 y;//!< Y成分
        f32 z;//!< Z成分
        f32 w;//!< W成分

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief デフォルトコンストラクタ
    //@―---------------------------------------------------------------------------
    inline Quat::Quat() noexcept {
        SetIdentity();
    };


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
        Set(roll, pitch, yaw);
    }


    //@―---------------------------------------------------------------------------
    //! @brief			コンストラクタ(任意軸回転)
    //! 
    //! @param rotation オイラー角
    //@―---------------------------------------------------------------------------
    inline Quat::Quat(f32 angle, const Vec3& axis) {
        Set(angle, axis);
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
        return (*this) * quat.Inverse();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子
    //@―---------------------------------------------------------------------------
    inline Quat Quat::operator /= (const Quat& quat) noexcept {
        (*this) *= quat.Inverse();
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Quat::operator == (const Quat& quat)const noexcept {
        return
            Mathf::Approximately(x, quat.x) &&
            Mathf::Approximately(y, quat.y) &&
            Mathf::Approximately(z, quat.z) &&
            Mathf::Approximately(w, quat.w);
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
    inline void Quat::Rotate(f32 x, f32 y, f32 z) {
        (*this) = Quat(x, y, z) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカル回転
    //@―---------------------------------------------------------------------------
    inline void Quat::RotateLocal(f32 x, f32 y, f32 z) {
        (*this) *= Quat(x, y, z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバルX軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::RotateX(f32 degrees) {
        (*this) = Quat(degrees, 0, 0) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバルY軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::RotateY(f32 degrees) {
        (*this) = Quat(0, degrees, 0) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバルZ軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::RotateZ(f32 degrees) {
        (*this) = Quat(0, 0, degrees) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカルX軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::RotateLocalX(f32 degrees) {
        (*this) *= Quat(degrees, 0, 0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカルY軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::RotateLocalY(f32 degrees) {
        (*this) *= Quat(0, degrees, 0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカルZ軸回転
    //@―---------------------------------------------------------------------------
    inline void Quat::RotateLocalZ(f32 degrees) {
        (*this) *= Quat(0, 0, degrees);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 逆回転を表すQuaternionを取得
    //@―---------------------------------------------------------------------------
    inline Quat Quat::Inverse() const noexcept {
        return Quat(-x, -y, -z, w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ロール・ピッチ・ヨー設定
    //@―---------------------------------------------------------------------------
    inline void Quat::Set(f32 x_, f32 y_, f32 z_, f32 w_) noexcept {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ロール・ピッチ・ヨー設定
    //@―---------------------------------------------------------------------------
    inline void Quat::Set(f32 roll, f32 pitch, f32 yaw) {
        f32 sx = Mathf::Sin(Mathf::Degrees(roll * 0.5f));
        f32 cx = Mathf::Cos(Mathf::Degrees(roll * 0.5f));
        f32 sy = Mathf::Sin(Mathf::Degrees(pitch * 0.5f));
        f32 cy = Mathf::Cos(Mathf::Degrees(pitch * 0.5f));
        f32 sz = Mathf::Sin(Mathf::Degrees(yaw * 0.5f));
        f32 cz = Mathf::Cos(Mathf::Degrees(yaw * 0.5f));
        //this->x = cy * sx * cz - sy * cx * sz;
        //this->y = cy * sx * sz + sy * cx * sz;
        //this->z = cy * cx * sz + sy * sx * cz;
        //this->w = cy * cx * cz - sy * sx * sz;
        *this = Quat(0, sy, 0, cy) * Quat(sx, 0, 0, cx) * Quat(0, 0, sz, cz);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転なし状態に設定
    //@―---------------------------------------------------------------------------
    inline void Quat::SetIdentity() noexcept {
        Set(0.0f, 0.0f, 0.0f, 1.0f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの長さの取得
    //! 
    //! @details 二つの Quat の長さを比較する場合はSqrMagのほうが高速です。
    //@―---------------------------------------------------------------------------
    inline f32 Quat::GetMag() const {
        return Mathf::Sqrt(GetSqrMag());
    }

    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの長さの二乗を取得
    //@―---------------------------------------------------------------------------
    inline f32 Quat::GetSqrMag() const noexcept {
        return x * x + y * y + z * z + w * w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 正規化
    //! 
    //! @note 計算誤差によりノルムが1でなくなった場合は明示的に正規化する必要がある
    //@―---------------------------------------------------------------------------
    inline void Quat::Normalize() {
        f32 div = 1.0f / Mathf::Sqrt(x * x + y * y + z * z + w * w);
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
    inline Quat Quat::GetNorm()const {
        f32 div = 1.0f / Mathf::Sqrt(x * x + y * y + z * z + w * w);
        return Quat(x * div, y * div, z * div, w * div);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転量を取得
    //@―---------------------------------------------------------------------------
    inline f32 Quat::Angle() const {
        return Mathf::Radians(Mathf::Acos(w) * 2.0f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 内積
    //@―---------------------------------------------------------------------------
    inline f32 Quat::Dot(const Quat& a, const Quat& b) noexcept {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価判定(許容範囲指定)
    //! 
    //! @param tolerance    許容範囲
    //@―---------------------------------------------------------------------------
    inline bool Quat::Equals(const Quat& v, f32 tolerance)const noexcept {
        return
            Mathf::Abs(x - v.x) <= tolerance &&
            Mathf::Abs(y - v.y) <= tolerance &&
            Mathf::Abs(z - v.z) <= tolerance &&
            Mathf::Abs(w - v.w) <= tolerance;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転を逆回転にする
    //@―---------------------------------------------------------------------------
    inline void Quat::Invert() noexcept {
        x *= -1.0f;
        y *= -1.0f;
        z *= -1.0f;
    }
}