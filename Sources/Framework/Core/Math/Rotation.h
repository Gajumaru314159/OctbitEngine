﻿//***********************************************************
//! @file
//! @brief		オイラー角回転構造体
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Math.h>

namespace ob::core {

    struct Vec3;
    struct Quat;

    //@―---------------------------------------------------------------------------
    //! @brief      オイラー回転を表す構造体
    //! 
    //! @details    回転はZXYの順に行われる。
    //@―---------------------------------------------------------------------------
    struct OB_API Rot {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Rot() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(roll/pitch/yaw指定)
        //@―---------------------------------------------------------------------------
        Rot(f32 roll, f32 pitch, f32 yaw)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(Vec3指定)
        //@―---------------------------------------------------------------------------
        explicit Rot(const Vec3& vec)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(Quat指定)
        //@―---------------------------------------------------------------------------
        explicit Rot(const Quat& quat);


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(roll/pitch/yaw指定)
        //@―---------------------------------------------------------------------------
        void set(f32 roll, f32 pitch, f32 yaw)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(Vec3指定)
        //@―---------------------------------------------------------------------------
        void set(const Vec3& vec)noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  Quatに変換
        //@―---------------------------------------------------------------------------
        Quat toQuat()const;

        //@―---------------------------------------------------------------------------
        //! @brief  Quatに変換
        //@―---------------------------------------------------------------------------
        Vec3 toVec3()const;


        //@―---------------------------------------------------------------------------
        //! @brief  正面ベクトルを取得
        //@―---------------------------------------------------------------------------
        Vec3 front()const;


        //@―---------------------------------------------------------------------------
        //! @brief  背面ベクトルを取得
        //@―---------------------------------------------------------------------------
        Vec3 back()const;


        //@―---------------------------------------------------------------------------
        //! @brief  上ベクトルを取得
        //@―---------------------------------------------------------------------------
        Vec3 up()const;


        //@―---------------------------------------------------------------------------
        //! @brief  下ベクトルを取得
        //@―---------------------------------------------------------------------------
        Vec3 down()const;


        //@―---------------------------------------------------------------------------
        //! @brief  左ベクトルを取得
        //@―---------------------------------------------------------------------------
        Vec3 left()const;


        //@―---------------------------------------------------------------------------
        //! @brief  右ベクトルを取得
        //@―---------------------------------------------------------------------------
        Vec3 right()const;


        //@―---------------------------------------------------------------------------
        //! @brief  正規化
        //@―---------------------------------------------------------------------------
        Rot& normalize();


        //@―---------------------------------------------------------------------------
        //! @brief  NaNを含むか
        //@―---------------------------------------------------------------------------
        bool isNaN();

    public:

        static const Rot zero;  //!< 無回転

    public:

        f32 roll;       //!< Z軸回転量
        f32 pitch;      //!< X軸回転量
        f32 yaw;        //!< Y軸回転量

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(roll/pitch/yaw指定)
    //@―---------------------------------------------------------------------------
    inline Rot::Rot(f32 roll, f32 pitch, f32 yaw)noexcept {
        set(roll, pitch, yaw);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(roll/pitch/yaw指定)
    //@―---------------------------------------------------------------------------
    inline void Rot::set(f32 roll, f32 pitch, f32 yaw)noexcept {
        this->roll = roll;
        this->pitch = pitch;
        this->yaw = yaw;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  正規化
    //@―---------------------------------------------------------------------------
    inline Rot& Rot::normalize() {
        roll = Math::Mod(roll, Math::PI);
        pitch = Math::Mod(pitch, Math::PI);
        yaw = Math::Mod(yaw, Math::PI);
        if (roll < 0.0f)roll += Math::TWO_PI;
        if (pitch < 0.0f)pitch += Math::TWO_PI;
        if (yaw < 0.0f)yaw += Math::TWO_PI;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  NaNを含むか
    //@―---------------------------------------------------------------------------
    inline bool Rot::isNaN() {
        return
            Math::IsNaN(roll) ||
            Math::IsNaN(pitch) ||
            Math::IsNaN(yaw);
    }

    //! @endcond
}// namespcae ob


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::Rot, ob::core::Char> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template<typename FormatContext>
    auto format(ob::core::Rot value, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(), TC("({},{},{})"), ob::Math::Degrees(value.roll), ob::Math::Degrees(value.pitch), ob::Math::Degrees(value.yaw));
    }
};
//! @endcond