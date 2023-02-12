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