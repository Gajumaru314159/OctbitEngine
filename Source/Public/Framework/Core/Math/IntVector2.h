﻿//***********************************************************
//! @file
//! @brief 整数二次元ベクトル
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Math.h>
#include <Framework/Core/Template/include.h>

namespace ob::core {

    struct Vec2;

    //@―---------------------------------------------------------------------------
    //! @brief 整数二次元ベクトル
    //@―---------------------------------------------------------------------------
    struct OB_API IntVec2 {
    public:

        s32 x;    //!< ベクトルのX成分
        s32 y;    //!< ベクトルのY成分

    public:

        static const IntVec2 Zero;    //!< ( 0, 0)
        static const IntVec2 One;     //!< ( 1, 1)
        static const IntVec2 Left;    //!< (-1, 0)
        static const IntVec2 Right;   //!< ( 1, 0)
        static const IntVec2 Down;    //!< ( 0,-1)
        static const IntVec2 Up;      //!< ( 0, 1)
        static const IntVec2 Minimum; //!< ( -inf, -inf)
        static const IntVec2 Maximum; //!< (  inf,  inf)

    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      デフォルトコンストラクタ(ゼロ初期化)
        //@―---------------------------------------------------------------------------
        IntVec2()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
        //@―---------------------------------------------------------------------------
        constexpr explicit IntVec2(s32 scalar)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(要素を指定して初期化)
        //@―---------------------------------------------------------------------------
        constexpr IntVec2(s32 x, s32 y)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ( Vec2 初期化)
        //@―---------------------------------------------------------------------------
        explicit IntVec2(const Vec2& vec)noexcept;


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator == (const IntVec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator != (const IntVec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec2 operator + (const IntVec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec2 operator + (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec2 operator - (const IntVec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec2 operator - (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec2 operator * (const IntVec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec2 operator * (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec2 operator / (const IntVec2& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec2 operator / (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec2& operator += (const IntVec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec2& operator += (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子 
        //@―---------------------------------------------------------------------------
        constexpr IntVec2& operator -= (const IntVec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec2& operator -= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子 
        //@―---------------------------------------------------------------------------
        constexpr IntVec2& operator *= (const IntVec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子 (スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec2& operator *= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子 
        //@―---------------------------------------------------------------------------
        constexpr IntVec2& operator /= (const IntVec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec2& operator /= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 単項マイナス演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec2 operator - () const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief s32*IntVec2演算子 
        //@―---------------------------------------------------------------------------
        friend IntVec2 operator * (s32 scalar, const IntVec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief s32/IntVec2演算子 
        //@―---------------------------------------------------------------------------
        friend IntVec2 operator / (s32 scalar, const IntVec2& v) noexcept;


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
        //! @brief Vec2 で取得
        //@―---------------------------------------------------------------------------
        Vec2    toVec2() const noexcept;


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
        static constexpr IntVec2 Abs(const IntVec2& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 内積
        //@―---------------------------------------------------------------------------
        static constexpr s32 Dot(const IntVec2& a, const IntVec2& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 外積
        //@―---------------------------------------------------------------------------
        static constexpr s32 Cross(const IntVec2& a, const IntVec2& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  二つのベクトルのマンハッタン距離を計算
        //@―---------------------------------------------------------------------------
        static constexpr s32 MDist(const IntVec2& a, const IntVec2& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
        //@―---------------------------------------------------------------------------
        static constexpr IntVec2 Max(const IntVec2& a, const IntVec2& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
        //@―---------------------------------------------------------------------------
        static constexpr IntVec2 Min(const IntVec2& a, const IntVec2& b) noexcept;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief      デフォルトコンストラクタ(初期化なし)
    //@―---------------------------------------------------------------------------
    inline IntVec2::IntVec2() noexcept
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
    //@―---------------------------------------------------------------------------
    constexpr IntVec2::IntVec2(s32 scalar) noexcept
        : IntVec2(scalar,scalar)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(要素を指定して初期化)
    //@―---------------------------------------------------------------------------
    constexpr IntVec2::IntVec2(s32 x, s32 y) noexcept 
        : x(x),y(y)
    {
    }


    //===============================================================
    //  オペレータ
    //===============================================================


    //@―---------------------------------------------------------------------------
    //! @brief      等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec2::operator == (const IntVec2& v) const noexcept {
        return
            x == v.x &&
            y == v.y;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec2::operator != (const IntVec2& v) const noexcept {
        return !(operator==(v));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::operator + (const IntVec2& v) const noexcept {
        return IntVec2(*this) += v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::operator + (s32 scalar) const noexcept {
        return IntVec2(*this) += scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::operator - (const IntVec2& v) const noexcept {
        return IntVec2(*this) -= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::operator - (s32 scalar) const noexcept {
        return IntVec2(*this) -= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::operator * (const IntVec2& v) const noexcept {
        return IntVec2(*this) *= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::operator * (s32 scalar) const noexcept {
        return IntVec2(*this) *= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::operator / (const IntVec2& v) const noexcept {
        return IntVec2(*this) /= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::operator / (s32 scalar) const noexcept {
        return IntVec2(*this) /= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec2& IntVec2::operator += (const IntVec2& v) noexcept {
        x += v.x;
        y += v.y;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec2& IntVec2::operator += (s32 scalar) noexcept {
        x += scalar;
        y += scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子 
    //@―---------------------------------------------------------------------------
    constexpr IntVec2& IntVec2::operator -= (const IntVec2& v) noexcept {
        x -= v.x;
        y -= v.y;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec2& IntVec2::operator -= (s32 scalar) noexcept {
        x -= scalar;
        y -= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子 
    //@―---------------------------------------------------------------------------
    constexpr IntVec2& IntVec2::operator *= (const IntVec2& v) noexcept {
        x *= v.x;
        y *= v.y;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子 (スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec2& IntVec2::operator *= (s32 scalar) noexcept {
        x *= scalar;
        y *= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子 
    //@―---------------------------------------------------------------------------
    constexpr IntVec2& IntVec2::operator /= (const IntVec2& v) noexcept {
        x /= v.x;
        y /= v.y;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec2& IntVec2::operator /= (s32 scalar) noexcept {
        x /= scalar;
        y /= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 単項マイナス演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::operator - () const noexcept {
        return IntVec2(-x, -y);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      要素取得
    //! 
    //! @details    範囲外の場合は0を返します。  
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec2::operator [](size_t index) const noexcept {
        switch (index) {
        case 0:return x;
        case 1:return y;
        default:return 0;
        }
    }


    //===============================================================
    //  ゲッター
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 要素の最大を取得
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec2::maxComponent() const noexcept {
        return  std::max(x, y);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の絶対値の最大を取得
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec2::maxAbsComponent() const noexcept {
        return std::max(abs(x), abs(y));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の最小を取得
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec2::minComponent() const noexcept {
        return std::min(x, y);
    }


    //===============================================================
    //  判定
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief すべての要素が同じ値か判定
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec2::allComponentsEqual()const noexcept {
        return x == y;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ゼロベクトルか判定
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec2::isZero()const noexcept {
        return x == 0 && y == 0;
    }


    //===============================================================
    //  スタティック関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの各要素の絶対値からベクトルを作成
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::Abs(const IntVec2& v) noexcept {
        return IntVec2(Math::Abs(v.x), Math::Abs(v.y));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 内積
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec2::Dot(const IntVec2& a, const IntVec2& b) noexcept {
        return a.x * b.x + a.y * b.y;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 外積
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec2::Cross(const IntVec2& a, const IntVec2& b) noexcept {
        return a.x * b.y - a.y * b.x;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  二つのベクトルのマンハッタン距離を計算
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec2::MDist(const IntVec2& a, const IntVec2& b) noexcept {
        return
            Math::Abs(a.x - b.x) +
            Math::Abs(a.y - b.y);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::Max(const IntVec2& a, const IntVec2& b) noexcept {
        return IntVec2(std::max(a.x, b.x), std::max(a.y, b.y));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
    //@―---------------------------------------------------------------------------
    constexpr IntVec2 IntVec2::Min(const IntVec2& a, const IntVec2& b) noexcept {
        return IntVec2(std::min(a.x, b.x), std::min(a.y, b.y));
    }

    //! @endcond
}// namespace ob::core


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::IntVec2, ob::core::Char> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template<typename FormatContext>
    auto format(ob::core::IntVec2 value, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(), TC("({},{})"), value.x, value.y);
    }
};
//! @endcond
