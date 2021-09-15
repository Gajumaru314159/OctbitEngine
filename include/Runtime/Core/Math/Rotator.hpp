//***********************************************************
//! @file
//! @brief		オイラー角回転構造体
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>


namespace ob
{

    struct Quat;
    struct Matrix;
    struct Vec3;

    //-----------------------------------------------------------
    //! @brief      オイラー回転を表す構造体
    //! 
    //! @details    回転はZXYの順に行われる。
    //-----------------------------------------------------------
    struct OB_API Rotator
    {
    public:

        Rotator(){}

        Rotator(f32 x_, f32 y_, f32 z_) :x(x_), y(y_), z(z_) {}

        explicit Rotator(const Quat& quat);

        explicit Rotator(const Matrix& mtx);

        Vec3 Front()const;    //!< 回転後の前ベクトル取得
        Vec3 Back()const;       //!< 回転後の後ベクトル取得
        Vec3 Up()const;         //!< 回転後の上ベクトル取得
        Vec3 Down()const;       //!< 回転後の下ベクトル取得
        Vec3 Left()const;       //!< 回転後の左ベクトル取得
        Vec3 Right()const;      //!< 回転後の右ベクトル取得

    public:

        static const Rotator zero;

    public:

        f32 x;
        f32 y;
        f32 z;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------


}// namespcae ob