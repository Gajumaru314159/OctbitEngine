//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/Transform.h>
#include <Framework/Core/Math/Matrix.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  Matrix に変換
    //@―---------------------------------------------------------------------------
    Matrix Transform::getMatrix()const {
        Matrix matrix;
        matrix.setIdentity();
        matrix.translate(position);
        matrix.rotate(rotation);
        matrix.scale(scale);
        return matrix;
    }

}// namespace ob