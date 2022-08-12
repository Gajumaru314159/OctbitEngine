﻿//***********************************************************
//! @file
//! @brief 整数三次元ベクトル
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Math.h>
#include <Framework/Core/Template/include.h>

namespace ob::core {

    struct Vec3;

    //@―---------------------------------------------------------------------------
    //! @brief 整数三次元ベクトル
    //@―---------------------------------------------------------------------------
    struct OB_API IntVec3 {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        IntVec3()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
        //@―---------------------------------------------------------------------------
        constexpr IntVec3(s32 scalar)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(要素を指定して初期化)
        //@―---------------------------------------------------------------------------
        constexpr IntVec3(s32 x, s32 y, s32 z)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ( Vec3 初期化)
        //@―---------------------------------------------------------------------------
        explicit IntVec3(const Vec3& vec)noexcept;


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator == (const IntVec3& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator != (const IntVec3& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec3 operator + (const IntVec3& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec3 operator + (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec3 operator - (const IntVec3& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec3 operator - (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec3 operator * (const IntVec3& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec3 operator * (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec3 operator / (const IntVec3& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec3 operator / (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec3& operator += (const IntVec3& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec3& operator += (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子 
        //@―---------------------------------------------------------------------------
        constexpr IntVec3& operator -= (const IntVec3& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec3& operator -= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子 
        //@―---------------------------------------------------------------------------
        constexpr IntVec3& operator *= (const IntVec3& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子 (スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec3& operator *= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子 
        //@―---------------------------------------------------------------------------
        constexpr IntVec3& operator /= (const IntVec3& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec3& operator /= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 単項マイナス演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec3 operator - () const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief s32*IntVec3演算子 
        //@―---------------------------------------------------------------------------
        friend IntVec3 operator * (s32 scalar, const IntVec3& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief s32/IntVec3演算子 
        //@―---------------------------------------------------------------------------
        friend IntVec3 operator / (s32 scalar, const IntVec3& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      要素取得
        //! 
        //! @details    範囲外の場合は0を返します。  
        //@―---------------------------------------------------------------------------
        constexpr s32 operator [](size_t index) const noexcept;


        //===============================================================
        //  変換
        //===============================================================


        //@―---------------------------------------------------------------------------
        //! @brief Vec3 で取得
        //@―---------------------------------------------------------------------------
        Vec3 toVec3() const noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 要素の最大を取得
        //@―---------------------------------------------------------------------------
        constexpr s32 maxComponent() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素の絶対値の最大を取得
        //@―---------------------------------------------------------------------------
        constexpr s32 maxAbsComponent() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素の最小を取得
        //@―---------------------------------------------------------------------------
        constexpr s32 minComponent() const noexcept;


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief すべての要素が同じ値か判定
        //@―---------------------------------------------------------------------------
        constexpr bool allComponentsEqual()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief ゼロベクトルか判定
        //@―---------------------------------------------------------------------------
        constexpr bool isZero()const noexcept;


    public:

        //===============================================================
        //  スタティック関数
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief ベクトルの各要素の絶対値からベクトルを作成
        //@―---------------------------------------------------------------------------
        static constexpr IntVec3 Abs(const IntVec3& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 内積
        //@―---------------------------------------------------------------------------
        static constexpr s32 Dot(const IntVec3& a, const IntVec3& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 外積
        //@―---------------------------------------------------------------------------
        static constexpr IntVec3 Cross(const IntVec3& a, const IntVec3& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  二つのベクトルのマンハッタン距離を計算
        //@―---------------------------------------------------------------------------
        static constexpr s32 MDist(const IntVec3& a, const IntVec3& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
        //@―---------------------------------------------------------------------------
        static constexpr IntVec3 Max(const IntVec3& a, const IntVec3& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
        //@―---------------------------------------------------------------------------
        static constexpr IntVec3 Min(const IntVec3& a, const IntVec3& b) noexcept;

    public:

        static const IntVec3 Zero;    //!< ( 0, 0, 0)
        static const IntVec3 One;     //!< ( 1, 1, 1)
        static const IntVec3 Left;    //!< (-1, 0, 0)
        static const IntVec3 Right;   //!< ( 1, 0, 0)
        static const IntVec3 Down;    //!< ( 0,-1, 0)
        static const IntVec3 Up;      //!< ( 0, 1, 0)
        static const IntVec3 Back;    //!< ( 0, 0,-1)
        static const IntVec3 Front;   //!< ( 0, 0, 1)
        static const IntVec3 Minimum; //!< (-inf,-inf,-inf)
        static const IntVec3 Maximum; //!< ( inf, inf, inf)

    public:

        s32 x;    //!< ベクトルのX成分
        s32 y;    //!< ベクトルのY成分
        s32 z;    //!< ベクトルのZ成分

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief      デフォルトコンストラクタ(初期化なし)
    //@―---------------------------------------------------------------------------
    inline IntVec3::IntVec3() noexcept
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
    //@―---------------------------------------------------------------------------
    constexpr IntVec3::IntVec3(s32 scalar) noexcept 
        : IntVec3(scalar,scalar,scalar)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(要素を指定して初期化)
    //@―---------------------------------------------------------------------------
    constexpr IntVec3::IntVec3(s32 x, s32 y, s32 z) noexcept 
        : x(x),y(y),z(z)
    {
    }


    //===============================================================
    //  オペレータ
    //===============================================================


    //@―---------------------------------------------------------------------------
    //! @brief      等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec3::operator == (const IntVec3& v) const noexcept {
        return
            x == v.x &&
            y == v.y &&
            z == v.z;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec3::operator != (const IntVec3& v) const noexcept {
        return !(operator==(v));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::operator + (const IntVec3& v) const noexcept {
        return IntVec3(*this) += v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::operator + (s32 scalar) const noexcept {
        return IntVec3(*this) += scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::operator - (const IntVec3& v) const noexcept {
        return IntVec3(*this) -= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::operator - (s32 scalar) const noexcept {
        return IntVec3(*this) -= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::operator * (const IntVec3& v) const noexcept {
        return IntVec3(*this) *= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::operator * (s32 scalar) const noexcept {
        return IntVec3(*this) *= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::operator / (const IntVec3& v) const noexcept {
        return IntVec3(*this) /= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::operator / (s32 scalar) const noexcept {
        return IntVec3(*this) /= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec3& IntVec3::operator += (const IntVec3& v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec3& IntVec3::operator += (s32 scalar) noexcept {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子 
    //@―---------------------------------------------------------------------------
    constexpr IntVec3& IntVec3::operator -= (const IntVec3& v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec3& IntVec3::operator -= (s32 scalar) noexcept {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子 
    //@―---------------------------------------------------------------------------
    constexpr IntVec3& IntVec3::operator *= (const IntVec3& v) noexcept {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子 (スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec3& IntVec3::operator *= (s32 scalar) noexcept {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子 
    //@―---------------------------------------------------------------------------
    constexpr IntVec3& IntVec3::operator /= (const IntVec3& v) noexcept {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec3& IntVec3::operator /= (s32 scalar) noexcept {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 単項マイナス演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::operator - () const noexcept {
        return IntVec3(-x, -y, -z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      要素取得
    //! 
    //! @details    範囲外の場合は0を返します。  
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec3::operator [](size_t index) const noexcept {
        switch (index) {
        case 0:return x;
        case 1:return y;
        case 2:return z;
        default:return 0;
        }
    }


    //===============================================================
    //  ゲッター
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 要素の最大を取得
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec3::maxComponent() const noexcept {
        return  std::max(std::max(x, y), z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の絶対値の最大を取得
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec3::maxAbsComponent() const noexcept {
        return std::max(std::max(abs(x), abs(y)), abs(z));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の最小を取得
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec3::minComponent() const noexcept {
        return std::min(std::min(abs(x), abs(y)), abs(z));
    }


    //===============================================================
    //  判定
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief すべての要素が同じ値か判定
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec3::allComponentsEqual()const noexcept {
        return x == y && y == z;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ゼロベクトルか判定
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec3::isZero()const noexcept {
        return x == 0 && y == 0 && z == 0;
    }


    //===============================================================
    //  スタティック関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの各要素の絶対値からベクトルを作成
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::Abs(const IntVec3& v) noexcept {
        return IntVec3(Math::Abs(v.x), Math::Abs(v.y), Math::Abs(v.z));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 内積
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec3::Dot(const IntVec3& a, const IntVec3& b) noexcept {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 外積
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::Cross(const IntVec3& a, const IntVec3& b) noexcept {
        return IntVec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  二つのベクトルのマンハッタン距離を計算
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec3::MDist(const IntVec3& a, const IntVec3& b) noexcept {
        return
            Math::Abs(a.x - b.x) +
            Math::Abs(a.y - b.y) +
            Math::Abs(a.z - b.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::Max(const IntVec3& a, const IntVec3& b) noexcept {
        return IntVec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
    //@―---------------------------------------------------------------------------
    constexpr IntVec3 IntVec3::Min(const IntVec3& a, const IntVec3& b) noexcept {
        return IntVec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }

    //! @endcond
}// namespace ob::core


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::IntVec3, ob::core::Char> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template<typename FormatContext>
    auto format(ob::core::IntVec3 value, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(), TC("({},{},{})"), value.x, value.y, value.z);
    }
};
//! @endcond
