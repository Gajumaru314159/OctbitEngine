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
        Quat()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief		コンストラクタ(内部値指定)
        //! 
        //! @details	与えられた x、y、z、w 成分で新規のクォータニオンを作成します。
        //! @deprecated	成分は複素数に基づいているため、通常は使用しない関数です。
        //@―---------------------------------------------------------------------------
        constexpr Quat(f32 x, f32 y, f32 z, f32 w)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief			コンストラクタ(オイラー角)
        //! 
        //! @param x		X軸回転(Deg)
        //! @param y	    Y軸回転(Deg)
        //! @param z		Z軸回転(Deg)
        //@―---------------------------------------------------------------------------
        Quat(f32 x, f32 y, f32 z);


        //@―---------------------------------------------------------------------------
        //! @brief			コンストラクタ(オイラー角)
        //! 
        //! @param rotation オイラー角(Deg)
        //@―---------------------------------------------------------------------------
        explicit Quat(const Rot& rotation);


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ(軸回転)
        //! 
        //! @param axis     回転軸
        //! @param angle    回転量(Deg)
        //@―---------------------------------------------------------------------------
        Quat(Vec3 axis, f32 angle);


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      乗算演算子
        //@―---------------------------------------------------------------------------
        constexpr Quat operator *  (const Quat& quat) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      乗算代入演算子
        //@―---------------------------------------------------------------------------
        constexpr Quat& operator *= (const Quat& quat) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      乗算演算子
        //@―---------------------------------------------------------------------------
        Quat operator *  (f32 scalar) const;


        //@―---------------------------------------------------------------------------
        //! @brief      乗算代入演算子
        //@―---------------------------------------------------------------------------
        Quat& operator *= (f32 scalar);


        //@―---------------------------------------------------------------------------
        //! @brief      除算演算子
        //@―---------------------------------------------------------------------------
        constexpr Quat operator / (const Quat& quat) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      除算代入演算子
        //@―---------------------------------------------------------------------------
        constexpr Quat& operator /= (const Quat& quat) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator == (const Quat& rhs)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator != (const Quat& rhs)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      ベクトル乗算演算子(回転)
        //@―---------------------------------------------------------------------------
        Vec3 operator *  (const Vec3& v) const noexcept;


        //===============================================================
        //  変換
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 行列表現を取得
        //@―---------------------------------------------------------------------------
        Matrix toMatrix() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief オイラー角での表現を取得
        //@―---------------------------------------------------------------------------
        Rot toRot() const;


        //@―---------------------------------------------------------------------------
        //! @brief 回転軸と回転量を取得
        //@―---------------------------------------------------------------------------
        auto toAxisAndAngle() const->std::tuple<Vec3, f32>;


        //@―---------------------------------------------------------------------------
        //! @brief 回転軸と回転量を取得
        //@―---------------------------------------------------------------------------
        auto toSwingTwist(const Vec3& twistAxis) const->std::tuple<Quat, Quat>;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 回転後の前ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3 front()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転後の後ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3 back()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転後の上ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3 up()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転後の下ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3 down()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転後の左ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3 left()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転後の右ベクトル取得
        //@―---------------------------------------------------------------------------
        Vec3 right()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief ベクトルの長さを取得
        //! 
        //! @details 二つの Quat の長さを比較する場合はSqrMagのほうが高速です。
        //@―---------------------------------------------------------------------------
        f32 length() const;


        //@―---------------------------------------------------------------------------
        //! @brief ベクトルの長さの二乗を取得
        //@―---------------------------------------------------------------------------
        f32 sqrLength() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 回転軸を取得
        //! 
        //! @return 回転量が0の場合はVector3(0,1,0)を返す。
        //@―---------------------------------------------------------------------------
        Vec3 axis() const;


        //@―---------------------------------------------------------------------------
        //! @brief 回転量を取得
        //@―---------------------------------------------------------------------------
        f32 angle() const;


        //@―---------------------------------------------------------------------------
        //! @brief 逆回転を表すQuaternionを取得
        //@―---------------------------------------------------------------------------
        constexpr Quat inverse() const noexcept;


        //===============================================================
        //  操作
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 正規化
        //! 
        //! @note 計算誤差によりノルムが1でなくなった場合は明示的に正規化する必要がある
        //@―---------------------------------------------------------------------------
        Quat& normalize ();


        //@―---------------------------------------------------------------------------
        //! @brief グローバル回転
        //@―---------------------------------------------------------------------------
        Quat& rotate(Rot rotation);


        //@―---------------------------------------------------------------------------
        //! @brief グローバル回転
        //@―---------------------------------------------------------------------------
        Quat& rotate(f32 x, f32 y, f32 z);


        //@―---------------------------------------------------------------------------
        //! @brief ローカル回転
        //@―---------------------------------------------------------------------------
        Quat& rotateLocal(Rot rotation);


        //@―---------------------------------------------------------------------------
        //! @brief ローカル回転
        //@―---------------------------------------------------------------------------
        Quat& rotateLocal(f32 x, f32 y, f32 z);


        //@―---------------------------------------------------------------------------
        //! @brief グローバルX軸回転
        //@―---------------------------------------------------------------------------
        Quat& rotateX(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief グローバルY軸回転
        //@―---------------------------------------------------------------------------
        Quat& rotateY(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief グローバルZ軸回転
        //@―---------------------------------------------------------------------------
        Quat& rotateZ(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief ローカルX軸回転
        //@―---------------------------------------------------------------------------
        Quat& rotateLocalX(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief ローカルY軸回転
        //@―---------------------------------------------------------------------------
        Quat& rotateLocalY(f32 degrees);


        //@―---------------------------------------------------------------------------
        //! @brief ローカルZ軸回転
        //@―---------------------------------------------------------------------------
        Quat& rotateLocalZ(f32 degrees);


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 等価判定(許容誤差指定)
        //! 
        //! @param v            比較対象
        //! @param tolerance    許容誤差
        //@―---------------------------------------------------------------------------
        constexpr bool equals(Quat v, f32 tolerance = Math::TOLERANCE)const noexcept;         // 等価判定(許容誤差指定)


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
        static constexpr f32 Dot(const Quat& a, const Quat& b) noexcept;

    public:

        static const Quat Identity;     //!< 単位元

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
    //! @brief デフォルトコンストラクタ(初期化なし)
    //@―---------------------------------------------------------------------------
    inline Quat::Quat()noexcept
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief		コンストラクタ(内部値指定)
    //! 
    //! @details	与えられた x、y、z、w 成分で新規のクォータニオンを作成します。
    //! @deprecated	成分は複素数に基づいているため、通常は使用しない関数です。
    //@―---------------------------------------------------------------------------
    constexpr Quat::Quat(f32 x, f32 y, f32 z, f32 w)  noexcept
        :x(x), y(y), z(z), w(w) {
    };


    //@―---------------------------------------------------------------------------
    //! @brief      乗算演算子
    //@―---------------------------------------------------------------------------
    constexpr Quat Quat::operator * (const Quat& quat) const noexcept {
        return Quat(*this) *= quat;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      乗算代入演算子
    //@―---------------------------------------------------------------------------
    constexpr Quat& Quat::operator *= (const Quat& quat) noexcept {
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
    constexpr Quat Quat::operator / (const Quat& quat) const noexcept {
        return Quat(*this) /= quat;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子
    //@―---------------------------------------------------------------------------
    constexpr Quat& Quat::operator /= (const Quat& quat) noexcept {
        return *this *= quat.inverse();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool Quat::operator == (const Quat& quat)const noexcept {
        return
            Math::IsNearEquals(x, quat.x) &&
            Math::IsNearEquals(y, quat.y) &&
            Math::IsNearEquals(z, quat.z) &&
            Math::IsNearEquals(w, quat.w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool Quat::operator != (const Quat& quat)const noexcept {
        return !(operator==(quat));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 逆回転を表すQuaternionを取得
    //@―---------------------------------------------------------------------------
    constexpr Quat Quat::inverse() const noexcept {
        return Quat(-x, -y, -z, w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転量を取得
    //@―---------------------------------------------------------------------------
    inline f32 Quat::angle() const {
        return Math::Radians(Math::Acos(w) * 2.0f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバル回転
    //@―---------------------------------------------------------------------------
    inline Quat& Quat::rotate(f32 x, f32 y, f32 z) {
        return *this = Quat(x, y, z) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカル回転
    //@―---------------------------------------------------------------------------
    inline Quat& Quat::rotateLocal(f32 x, f32 y, f32 z) {
        return *this *= Quat(x, y, z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバルX軸回転
    //@―---------------------------------------------------------------------------
    inline Quat& Quat::rotateX(f32 degrees) {
        return *this = Quat(degrees, 0, 0) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバルY軸回転
    //@―---------------------------------------------------------------------------
    inline Quat& Quat::rotateY(f32 degrees) {
        return  *this = Quat(0, degrees, 0) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバルZ軸回転
    //@―---------------------------------------------------------------------------
    inline Quat& Quat::rotateZ(f32 degrees) {
        return *this = Quat(0, 0, degrees) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカルX軸回転
    //@―---------------------------------------------------------------------------
    inline Quat& Quat::rotateLocalX(f32 degrees) {
        return *this *= Quat(degrees, 0, 0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカルY軸回転
    //@―---------------------------------------------------------------------------
    inline Quat& Quat::rotateLocalY(f32 degrees) {
        return *this *= Quat(0, degrees, 0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカルZ軸回転
    //@―---------------------------------------------------------------------------
    inline Quat& Quat::rotateLocalZ(f32 degrees) {
        return *this *= Quat(0, 0, degrees);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 内積
    //@―---------------------------------------------------------------------------
    constexpr f32 Quat::Dot(const Quat& a, const Quat& b) noexcept {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価判定(許容誤差指定)
    //! 
    //! @param tolerance    許容誤差
    //@―---------------------------------------------------------------------------
    constexpr bool Quat::equals(Quat v, f32 tolerance)const noexcept {
        return
            Math::Abs(x - v.x) <= tolerance &&
            Math::Abs(y - v.y) <= tolerance &&
            Math::Abs(z - v.z) <= tolerance &&
            Math::Abs(w - v.w) <= tolerance;
    }

    //! @endcond
}


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::Quat, ob::core::Char> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template<typename FormatContext>
    auto format(ob::core::Quat value, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(), TC("({},{},{},{})"), value.x, value.y, value.z, value.w);
    }
};
//! @endcond