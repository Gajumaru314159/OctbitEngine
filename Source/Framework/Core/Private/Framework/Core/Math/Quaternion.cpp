//***********************************************************
//! @file
//! @brief Quaternion
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Math/Quaternion.h>
#include <Framework/Core/Math/Math.h>
#include <Framework/Core/Math/Matrix.h>
#include <Framework/Core/Math/Rotation.h>
#include <Framework/Core/Math/Vectors.h>

namespace ob::core {

    const Quat Quat::Identity = Quat(0,0,0,1);

    //===============================================================
    //  コンストラクタ / デストラクタ
    //===============================================================


    //@―---------------------------------------------------------------------------
    //! @brief			コンストラクタ(オイラー角)
    //! 
    //! @param x		X軸回転(Deg)
    //! @param y	    Y軸回転(Deg)
    //! @param z		Z軸回転(Deg)
    //@―---------------------------------------------------------------------------
    Quat::Quat(f32 x, f32 y, f32 z) {
        f32 sx = Math::SinD(x * 0.5f);
        f32 cx = Math::CosD(x * 0.5f);
        f32 sy = Math::SinD(y * 0.5f);
        f32 cy = Math::CosD(y * 0.5f);
        f32 sz = Math::SinD(z * 0.5f);
        f32 cz = Math::CosD(z * 0.5f);
        *this = Quat(0, sy, 0, cy) * Quat(sx, 0, 0, cx) * Quat(0, 0, sz, cz);
    }


    //@―---------------------------------------------------------------------------
    //! @brief			コンストラクタ(オイラー角)
    //! 
    //! @param rotation オイラー角(Deg)
    //@―---------------------------------------------------------------------------
    Quat::Quat(const Rot& rotation)
        : Quat(rotation.x,rotation.y,rotation.z)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタ(軸回転)
    //! 
    //! @param axis     回転軸
    //! @param angle    回転量(Deg)
    //@―---------------------------------------------------------------------------
    Quat::Quat(Vec3 axis, f32 angle) {
        f32 s, c;
        Math::SinCos(Math::Degrees(angle * 0.5f), s, c);
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
        w = c;
        f32 magDiv = 1.0f / axis.length();
        x *= magDiv;
        y *= magDiv;
        z *= magDiv;
        w *= magDiv;
    }


    //===============================================================
    //  オペレータ
    //===============================================================


    //@―---------------------------------------------------------------------------
    //! @brief ベクトル乗算演算子(回転)
    //@―---------------------------------------------------------------------------
    Vec3 Quat::operator *  (const Vec3& v) const noexcept {
        Quat vec(v.x, v.y, v.z, 0.0f);
        vec = (*this) * vec * inverse();
        return Vec3(vec.x, vec.y, vec.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      乗算演算子
    //@―---------------------------------------------------------------------------
    Quat Quat::operator * (f32 scalar) const {
        Quat q(*this);
        q *= scalar;
        return q;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      乗算代入演算子
    //@―---------------------------------------------------------------------------
    Quat& Quat::operator *= (f32 scalar) {
        auto [axis,angle] = toAxisAndAngle();
        return *this = Quat(axis,angle);
    }


    //===============================================================
    //  変換
    //===============================================================


    //@―---------------------------------------------------------------------------
    //! @brief 行列表現を取得
    //@―---------------------------------------------------------------------------
    Matrix Quat::toMatrix()const noexcept {
        const f32 ww = w * w;
        const f32 xx = x * x;
        const f32 yy = y * y;
        const f32 zz = z * z;

        return Matrix(
            ww + xx - yy - zz, 2 * (x * y - w * z), 2 * (w * y + x * z), 0.0,
            2 * (x * y + w * z), ww - xx + yy - zz, 2 * (y * z - w * x), 0.0,
            2 * (x * z - w * y), 2 * (y * z + w * x), ww - xx - yy + zz, 0.0,
            0.0, 0.0, 0.0, 1.0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief オイラー角での表現を取得
    //@―---------------------------------------------------------------------------
    Rot Quat::toRot()const {
        f32 tx, ty, tz;
        f32 sx = -2 * (y * z - w * x);
        if (Math::Abs(sx - 1.0f) < Math::EPSILON) {
            tx = Math::HALF_PI;
            ty = Math::Atan2(2 * (x * y - w * z), w * w + x * x - y * y - z * z);
            tz = 0;
        } else if (Math::Abs(sx + 1.0f) < Math::EPSILON) {
            tx = -Math::HALF_PI;
            ty = Math::Atan2(-2 * (x * y - w * z), w * w + x * x - y * y - z * z);
            tz = 0;
        } else {
            tx = Math::Asin(-2 * (y * z - w * x));
            ty = Math::Atan2(2 * (w * y + x * z), w * w - x * x - y * y + z * z);
            tz = Math::Atan2(2 * (x * y + w * z), w * w - x * x + y * y - z * z);
        }

        return Rot(Math::Radians(tx), Math::Radians(ty), Math::Radians(tz));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転軸と回転量を取得
    //@―---------------------------------------------------------------------------
    auto Quat::toAxisAndAngle() const -> std::tuple<Vec3, f32> {
        f32 sin = Math::Sqrt(1.0f - w * w);
        if (sin <= Math::EPSILON)return { Vec3::Up,0.0f };

        f32 div = 1.0f / sin;

        return {
            Vec3(x * div, y * div, z * div),
            Math::Radians(Math::Acos(w) * 2.0f)
        };
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転軸と回転量を取得
    //@―---------------------------------------------------------------------------
    auto Quat::toSwingTwist(const Vec3& twistAxis) const->std::tuple<Quat, Quat> {

        Vec3 Projection = Vec3::Dot(twistAxis, Vec3(x, y, z)) * twistAxis;

        auto outTwist = Quat(Projection.x, Projection.y, Projection.z, w);

        if (outTwist.sqrLength() <= Math::EPSILON) {
            outTwist = Quat::Identity;
        } else {
            outTwist.normalize();
        }

        return {
            outTwist,
            (*this) * outTwist.inverse()
        };
    }


    //===============================================================
    //  ゲッター
    //===============================================================


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の前ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::front()const noexcept {
        return (*this) * Vec3::Front;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の後ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::back()const noexcept {
        return (*this) * Vec3::Back;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の上ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::up()const noexcept {
        return (*this) * Vec3::Up;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の下ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::down()const noexcept {
        return (*this) * Vec3::Down;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の左ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::left()const noexcept {
        return (*this) * Vec3::Left;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の右ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::right()const noexcept {
        return (*this) * Vec3::Right;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの長さを取得
    //! 
    //! @details 二つの Quat の長さを比較する場合はSqrMagのほうが高速です。
    //@―---------------------------------------------------------------------------
    f32 Quat::length() const {
        return Math::Sqrt(sqrLength());
    }


    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの長さの二乗を取得
    //@―---------------------------------------------------------------------------
    f32 Quat::sqrLength() const noexcept {
        return x * x + y * y + z * z + w * w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転軸を取得
    //! 
    //! @return 回転量が0の場合はVector3(0.0f,1.0f,0.0f)を返す。
    //@―---------------------------------------------------------------------------
    Vec3 Quat::axis() const {
        f32 sin = Math::Sqrt(1.0f - w * w);
        if (sin <= Math::EPSILON)return Vec3(0.0f, 1.0f, 0.0f);
        f32 div = 1.0f / sin;
        return Vec3(x * div, y * div, z * div);
    }


    //===============================================================
    //  操作
    //===============================================================


    //@―---------------------------------------------------------------------------
    //! @brief 正規化
    //! 
    //! @note 計算誤差によりノルムが1でなくなった場合は明示的に正規化する必要がある
    //@―---------------------------------------------------------------------------
    Quat& Quat::normalize() {
        f32 div = 1.0f / Math::Sqrt(x * x + y * y + z * z + w * w);
        return *this = Quat(x * div, y * div, z * div, w * div);
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバル回転
    //@―---------------------------------------------------------------------------
    Quat& Quat::rotate(Rot rotation) {
        return *this = Quat(rotation) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカル回転
    //@―---------------------------------------------------------------------------
    Quat& Quat::rotateLocal(Rot rotation) {
        return *this *= Quat(rotation);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 方向ベクトルをfromDirからtoDirに回転するQuaternionの計算
    //@―---------------------------------------------------------------------------
    Quat Quat::FromTo(const Vec3& fromDir, const Vec3& toDir) {
        Vec3 fromDirN = fromDir.unitVec();
        Vec3 toDirN = toDir.unitVec();
        Vec3 axis = Vec3::Cross(fromDirN, toDirN);
        f32 angle = Vec3::Dot(fromDirN, toDirN);
        return Quat(axis, angle);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ターゲット方向に向けるQuaternionを計算
    //@―---------------------------------------------------------------------------
    Quat Quat::LookAt(const Vec3& target, const Vec3& up) {
        Vec3 tar = target.unitVec();
        Vec3 norm(0, 0, 1);
        f32 dot = Vec3::Dot(norm, tar);
        f32 theta = Math::Acos(dot);
        Vec3 cross = Vec3::Cross(norm, tar);
        cross.normalize();
        theta = theta / 2;

        f32 sin = Math::Sin(theta);
        f32 cos = Math::Cos(theta);
        return Quat(cross.x * sin, cross.y * sin, cross.z * sin, cos);
    }


    //@―---------------------------------------------------------------------------
    //! @brief Quaternionの線形補完
    //@―---------------------------------------------------------------------------
    Quat Quat::Lerp(const Quat& a, const Quat& b, f32 t) {
        if (t <= 0.0f)return a;
        if (1.0f <= t)return b;
        return Quat(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t).normalize();
    }


    //@―---------------------------------------------------------------------------
    //! @brief Quaternionの球面補完
    //@―---------------------------------------------------------------------------
    Quat Quat::Slerp(const Quat& a, const Quat& b, f32 t) {
        if (t <= 0.0f)return a;
        if (1.0f <= t)return b;

        f32 dot = Quat::Dot(a, b);
        f32 theta = Math::Acos(dot);

        if (theta < 0.0)theta = -theta;

        f32 st = Math::Sin(theta);

        if (st == 0)return a;

        f32 sut = Math::Sin(theta * t);
        f32 sout = Math::Sin(theta * (1.0f - t));

        f32 coeff1 = sout / st;
        f32 coeff2 = sut / st;

        f32 x = coeff1 * a.x + coeff2 * b.x;
        f32 y = coeff1 * a.y + coeff2 * b.y;
        f32 z = coeff1 * a.z + coeff2 * b.z;
        f32 w = coeff1 * a.w + coeff2 * b.w;

        return Quat(x, y, z, w);
    }

}