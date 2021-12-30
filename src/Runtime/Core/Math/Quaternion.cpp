//***********************************************************
//! @file
//! @brief Quaternion
//! @author Gajumaru
//***********************************************************
#include <Runtime/Core/Math/Quaternion.h>
#include <Runtime/Core/Math/Mathf.h>
#include <Runtime/Core/Math/Matrix.h>
#include <Runtime/Core/Math/Vector/include.h>

namespace ob {

    const Quat Quat::identity = Quat(EForceInit::Force);


    //@―---------------------------------------------------------------------------
    //! @brief			コンストラクタ
    //! 
    //! @param rotation オイラー角
    //@―---------------------------------------------------------------------------
    Quat::Quat(const Vec3& eulerAngles) :Quat(eulerAngles.x, eulerAngles.y, eulerAngles.z) {}



    //@―---------------------------------------------------------------------------
    //! @brief ベクトル乗算演算子(回転)
    //@―---------------------------------------------------------------------------
    Vec3 Quat::operator *  (const Vec3& v) const noexcept {
        Quat vec(v.x, v.y, v.z, 0.0f);
        vec = (*this) * vec * GetInverse();
        return Vec3(vec.x, vec.y, vec.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      乗算演算子
    //@―---------------------------------------------------------------------------
    inline Quat Quat::operator * (f32 scalar) const {
        Quat q(*this);
        q *= scalar;
        return q;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      乗算代入演算子
    //@―---------------------------------------------------------------------------
    inline Quat Quat::operator *= (f32 scalar) {
        f32 angle;
        Vec3 axis;
        ToAxisAndAngle(angle, axis);
        Set(angle * scalar, axis);
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ロール・ピッチ・ヨー設定
    //@―---------------------------------------------------------------------------
    void Quat::Set(const Vec3& rotation) {
        Set(rotation.x, rotation.y, rotation.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 任意軸回転設定
    //@―---------------------------------------------------------------------------
    void Quat::Set(f32 angle, const Vec3& axis) {
        f32 s, c;
        Mathf::SinCos(Mathf::Degrees(angle * 0.5f), s, c);
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
        w = c;
        f32 magDiv = 1.0f / Mathf::Sqrt(x * x + y * y + z * z + w * w);
        x /= magDiv;
        y /= magDiv;
        z /= magDiv;
        w /= magDiv;
    }


    //@―---------------------------------------------------------------------------
    //! @brief グローバル回転
    //@―---------------------------------------------------------------------------
    void Quat::Rotate(const Vec3& eulerAngles) {
        (*this) = Quat(eulerAngles) * (*this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ローカル回転
    //@―---------------------------------------------------------------------------
    void Quat::RotateLocal(const Vec3& eulerAngles) {
        (*this) *= Quat(eulerAngles);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の前ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::GetFront()const noexcept {
        return (*this) * Vec3::front;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の後ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::GetBack()const noexcept {
        return (*this) * Vec3::back;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の上ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::GetUp()const noexcept {
        return (*this) * Vec3::up;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の下ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::GetDown()const noexcept {
        return (*this) * Vec3::down;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の左ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::GetLeft()const noexcept {
        return (*this) * Vec3::left;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転後の右ベクトル取得
    //@―---------------------------------------------------------------------------
    Vec3 Quat::GetRight()const noexcept {
        return (*this) * Vec3::right;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列表現を取得
    //@―---------------------------------------------------------------------------
    Matrix Quat::GetMatrix()const noexcept {
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
    Vec3 Quat::GetRot()const {
        f32 tx, ty, tz;
        f32 sx = -2 * (y * z - w * x);
        if (Mathf::Abs(sx - 1.0f) < Mathf::EPSILON) {
            tx = Mathf::HALF_PI;
            ty = Mathf::Atan2(2 * (x * y - w * z), w * w + x * x - y * y - z * z);
            tz = 0;
        } else if (Mathf::Abs(sx + 1.0f) < Mathf::EPSILON) {
            tx = -Mathf::HALF_PI;
            ty = Mathf::Atan2(-2 * (x * y - w * z), w * w + x * x - y * y - z * z);
            tz = 0;
        } else {
            tx = Mathf::Asin(-2 * (y * z - w * x));
            ty = Mathf::Atan2(2 * (w * y + x * z), w * w - x * x - y * y + z * z);
            tz = Mathf::Atan2(2 * (x * y + w * z), w * w - x * x + y * y - z * z);
        }

        return Vec3(tx, ty, tz) * (180.0f / Mathf::PI);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転軸を取得
    //! 
    //! @return 回転量が0の場合はVector3(0.0f,1.0f,0.0f)を返す。
    //@―---------------------------------------------------------------------------
    Vec3 Quat::GetAxis() const {
        f32 sin = Mathf::Sqrt(1.0f - w * w);
        if (sin <= Mathf::EPSILON)return Vec3(0.0f, 1.0f, 0.0f);
        f32 div = 1.0f / sin;
        return Vec3(x * div, y * div, z * div);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転軸と回転量を取得
    //@―---------------------------------------------------------------------------
    void Quat::ToAxisAndAngle(f32& angle, Vec3& axis) const {
        f32 sin = Mathf::Sqrt(1.0f - w * w);
        if (sin <= Mathf::EPSILON) {
            angle = 0.0f;
            axis = Vec3::up;
        }
        f32 div = 1.0f / sin;

        angle = Mathf::Radians(Mathf::Acos(w) * 2.0f);
        axis.Set(x * div, y * div, z * div);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 回転軸と回転量を取得
    //@―---------------------------------------------------------------------------
    void Quat::ToSwingTwist(const Vec3& twistAxis, Quat& outSwing, Quat& outTwist) const {
        // Vector part projected onto twist axis
        Vec3 Projection = Vec3::Dot(twistAxis, Vec3(x, y, z)) * twistAxis;

        // Twist quaternion
        outTwist = Quat(Projection.x, Projection.y, Projection.z, w);

        // Singularity close to 180deg
        if (outTwist.GetSqrMag() == 0.0f) {
            outTwist = Quat::identity;
        } else {
            outTwist.Normalize();
        }

        // Set swing
        outSwing = (*this) * outTwist.GetInverse();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 方向ベクトルをfromDirからtoDirに回転するQuaternionの計算
    //@―---------------------------------------------------------------------------
    Quat Quat::FromTo(const Vec3& fromDir, const Vec3& toDir) {
        Vec3 fromDirN = fromDir.GetUnitVec();
        Vec3 toDirN = toDir.GetUnitVec();
        Vec3 axis = Vec3::Cross(fromDirN, toDirN);
        f32 angle = Vec3::Dot(fromDirN, toDirN);
        return Quat(angle, axis);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ターゲット方向に向けるQuaternionを計算
    //@―---------------------------------------------------------------------------
    Quat Quat::LookAt(const Vec3& target, const Vec3& up) {
        Vec3 tar = target.GetUnitVec();
        Vec3 norm(0, 0, 1);
        f32 dot = Vec3::Dot(norm, tar);
        f32 theta = Mathf::Acos(dot);
        Vec3 cross = Vec3::Cross(norm, tar);
        cross.Normalize();
        theta = theta / 2;

        f32 sin = Mathf::Sin(theta);
        f32 cos = Mathf::Cos(theta);
        return Quat(cross.x * sin, cross.y * sin, cross.z * sin, cos);
    }


    //@―---------------------------------------------------------------------------
    //! @brief Quaternionの線形補完
    //@―---------------------------------------------------------------------------
    Quat Quat::Lerp(const Quat& a, const Quat& b, f32 t) {
        if (t <= 0.0f)return a;
        if (1.0f <= t)return b;
        return Quat(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t).GetNorm();
    }


    //@―---------------------------------------------------------------------------
    //! @brief Quaternionの球面補完
    //@―---------------------------------------------------------------------------
    Quat Quat::Slerp(const Quat& a, const Quat& b, f32 t) {
        if (t <= 0.0f)return a;
        if (1.0f <= t)return b;

        f32 dot = Quat::Dot(a, b);
        f32 theta = Mathf::Acos(dot);

        if (theta < 0.0)theta = -theta;

        f32 st = Mathf::Sin(theta);

        if (st == 0)return a;

        f32 sut = Mathf::Sin(theta * t);
        f32 sout = Mathf::Sin(theta * (1.0f - t));

        f32 coeff1 = sout / st;
        f32 coeff2 = sut / st;

        f32 x = coeff1 * a.x + coeff2 * b.x;
        f32 y = coeff1 * a.y + coeff2 * b.y;
        f32 z = coeff1 * a.z + coeff2 * b.z;
        f32 w = coeff1 * a.w + coeff2 * b.w;

        return Quat(x, y, z, w);
    }

}// namespace ob