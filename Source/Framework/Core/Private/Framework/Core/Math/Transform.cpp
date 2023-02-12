//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/Transform.h>
#include <Framework/Core/Math/Matrix.h>

namespace ob::core {

    const Transform Transform::Identity = { Vec3::Zero,Quat::Identity,Vec3::One };


    //@―---------------------------------------------------------------------------
    //! @brief  等価演算子
    //@―---------------------------------------------------------------------------
    bool Transform::operator==(const Transform& rhs)const noexcept {
        return
            position.equals(rhs.position) &&
            scale.equals(rhs.scale) &&
            rotation.equals(rhs.rotation);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  否等価演算子
    //@―---------------------------------------------------------------------------
    bool Transform::operator!=(const Transform& rhs)const noexcept {
        return !(*this == rhs);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  乗算
    //@―---------------------------------------------------------------------------
    Transform Transform::operator*(const Transform& rhs)const {
        Transform result;
        result.rotation = rotation * rhs.rotation;
        result.scale = scale * rhs.scale;
        result.position = toMatrix() * rhs.position;
        return result;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  乗算代入
    //@―---------------------------------------------------------------------------
    Transform Transform::operator*=(const Transform& rhs) {
        *this = *this * rhs;
        return *this;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  Matrix に変換
    //@―---------------------------------------------------------------------------
    Matrix Transform::toMatrix()const {
        return Matrix::TRS(position,rotation.toRot(), scale);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  反転
    //@―---------------------------------------------------------------------------
    Transform Transform::inverse()const {
        Transform out;
        out.rotation = rotation.inverse();
        out.scale = 1.0f / scale;
        out.position = -out.scale * (out.rotation * position);
        return out;
    }

}// namespace ob