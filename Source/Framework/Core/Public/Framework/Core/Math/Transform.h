//***********************************************************
//! @file
//! @brief		トランスフォーム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector3.h>
#include <Framework/Core/Math/Matrix.h>
#include <Framework/Core/Math/Rotation.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  トランスフォーム
    //@―---------------------------------------------------------------------------
    class Transform {
    public:

        bool operator==(const Transform&)const noexcept;
        bool operator!=(const Transform&)const noexcept;

        Transform operator*(const Transform&)const;
        Transform operator*=(const Transform&);

        //@―---------------------------------------------------------------------------
        //! @brief  Matrix に変換
        //@―---------------------------------------------------------------------------
        Matrix toMatrix()const;

        //@―---------------------------------------------------------------------------
        //! @brief  反転
        //@―---------------------------------------------------------------------------
        Transform inverse()const;

    public:

        static const Transform Identity;  //!< 初期状態

    public:

        Vec3    position;   //!< 位置
        Quat    rotation;   //!< 回転
        Vec3    scale;      //!< 拡縮

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob