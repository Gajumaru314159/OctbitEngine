﻿//***********************************************************
//! @file
//! @brief  Float4x4
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Matrix.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    struct float4x4{
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  デフォルトコンストラクタ
        //@―---------------------------------------------------------------------------
        float4x4() = default;

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @details    必要に応じて行列を転値して格納する
        //@―---------------------------------------------------------------------------
        constexpr float4x4(const Matrix& mtx)
            : matrix(mtx.transposed())
        {
        }

        //@―---------------------------------------------------------------------------
        //! @brief  Matrix の代入
        //! 
        //! @details    必要に応じて行列を転値して格納する
        //@―---------------------------------------------------------------------------
        float4x4& operator = (const Matrix& mtx)
        {
            matrix = mtx.transposed();
            return *this;
        }

    public:

        Matrix matrix;

    };

}// namespcae ob::core