//***********************************************************
//! @file
//! @brief		オイラー角回転構造体
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    struct Vec3;
    struct Quat;

    //@―---------------------------------------------------------------------------
    //! @brief      オイラー回転を表す構造体
    //! 
    //! @details    回転はZXYの順に行われる。
    //@―---------------------------------------------------------------------------
    struct OB_API Rotator {
    public:

        Rotator()noexcept {}
        explicit Rotator(const Quat& quat) {}
        explicit Rotator(const Vec3& vec)noexcept {}
        Rotator(f32 roll_, f32 pitch_, f32 yaw_)noexcept
            :roll(roll_), pitch(pitch_), yaw(yaw_) {
        }


        Vec3 Front()const;      //!< 回転後の前ベクトル取得
        Vec3 Back()const;       //!< 回転後の後ベクトル取得
        Vec3 Up()const;         //!< 回転後の上ベクトル取得
        Vec3 Down()const;       //!< 回転後の下ベクトル取得
        Vec3 Left()const;       //!< 回転後の左ベクトル取得
        Vec3 Right()const;      //!< 回転後の右ベクトル取得

        Rotator& Clamp();
        bool IsNaN();

    public:

        static const Rotator zero;

    public:

        f32 roll;       //!< Z軸回転量
        f32 pitch;      //!< X軸回転量
        f32 yaw;        //!< Y軸回転量

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------


}// namespcae ob