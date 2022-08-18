//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/Transform.h>
#include <Framework/Core/Math/Matrix.h>

namespace ob::core {

    const Transform Transform::Identity = { Vec3::Zero,Rot::Identity,Vec3::One };

    //@―---------------------------------------------------------------------------
    //! @brief  Matrix に変換
    //@―---------------------------------------------------------------------------
    Matrix Transform::toMatrix()const {
        return Matrix::TRS(position,rotation,scale);
    }

}// namespace ob