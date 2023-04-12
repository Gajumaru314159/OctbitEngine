﻿//***********************************************************
//! @file
//! @brief 整数四次元ベクトル
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Math.h>
#include <Framework/Core/Template/include.h>

namespace ob::core {

    struct Vec4;

    //@―---------------------------------------------------------------------------
    //! @brief 整数四次元ベクトル
    //@―---------------------------------------------------------------------------
    struct OB_API IntVec4 {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        IntVec4()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
        //@―---------------------------------------------------------------------------
        constexpr explicit IntVec4(s32 scalar)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(要素を指定して初期化)
        //@―---------------------------------------------------------------------------
        constexpr IntVec4(s32 x, s32 y, s32 z, s32 w)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ( Vec4 初期化)
        //@―---------------------------------------------------------------------------
        explicit IntVec4(const Vec4& vec)noexcept;


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator == (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator != (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec4 operator + (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec4 operator + (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec4 operator - (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec4 operator - (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec4 operator * (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec4 operator * (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec4 operator / (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec4 operator / (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec4& operator += (const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec4& operator += (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子 
        //@―---------------------------------------------------------------------------
        constexpr IntVec4& operator -= (const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec4& operator -= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子 
        //@―---------------------------------------------------------------------------
        constexpr IntVec4& operator *= (const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子 (スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec4& operator *= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子 
        //@―---------------------------------------------------------------------------
        constexpr IntVec4& operator /= (const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        constexpr IntVec4& operator /= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 単項マイナス演算子
        //@―---------------------------------------------------------------------------
        constexpr IntVec4 operator - () const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief s32*IntVec4演算子 
        //@―---------------------------------------------------------------------------
        friend IntVec4 operator * (s32 scalar, const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief s32/IntVec4演算子 
        //@―---------------------------------------------------------------------------
        friend IntVec4 operator / (s32 scalar, const IntVec4& v) noexcept;


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
        //! @brief Vec4 で取得
        //@―---------------------------------------------------------------------------
        Vec4    toVec4() const noexcept;


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
        constexpr  bool allComponentsEqual()const noexcept;


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
        static constexpr IntVec4  Abs(const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 内積
        //@―---------------------------------------------------------------------------
        static constexpr s32 Dot(const IntVec4& a, const IntVec4& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 外積
        //@―---------------------------------------------------------------------------
        static constexpr IntVec4  Cross(const IntVec4& a, const IntVec4& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  二つのベクトルのマンハッタン距離を計算
        //@―---------------------------------------------------------------------------
        static constexpr s32 MDist(const IntVec4& a, const IntVec4& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
        //@―---------------------------------------------------------------------------
        static constexpr IntVec4 Max(const IntVec4& a, const IntVec4& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
        //@―---------------------------------------------------------------------------
        static constexpr IntVec4 Min(const IntVec4& a, const IntVec4& b) noexcept;

    public:

        static const IntVec4 Zero;    //!< ( 0, 0, 0, 0)
        static const IntVec4 One;     //!< ( 1, 1, 1, 1)
        static const IntVec4 Minimum; //!< ( -inf, -inf, -inf, -inf)
        static const IntVec4 Maximum; //!< (  inf,  inf,  inf,  inf)

    public:

        s32 x;    //!< ベクトルのX成分
        s32 y;    //!< ベクトルのY成分
        s32 z;    //!< ベクトルのZ成分
        s32 w;    //!< ベクトルのW成分

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief      デフォルトコンストラクタ(初期化なし)
    //@―---------------------------------------------------------------------------
    inline IntVec4::IntVec4() noexcept
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
    //@―---------------------------------------------------------------------------
    constexpr IntVec4::IntVec4(s32 scalar) noexcept
        : IntVec4(scalar, scalar, scalar, scalar)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(要素を指定して初期化)
    //@―---------------------------------------------------------------------------
    constexpr IntVec4::IntVec4(s32 x, s32 y, s32 z, s32 w) noexcept
        : x(x),y(y),z(z),w(w)
    {
    }


    //===============================================================
    //  オペレータ
    //===============================================================


    //@―---------------------------------------------------------------------------
    //! @brief      等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec4::operator == (const IntVec4& v) const noexcept {
        return
            x == v.x &&
            y == v.y &&
            z == v.z &&
            w == v.w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec4::operator != (const IntVec4& v) const noexcept {
        return !(operator==(v));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::operator + (const IntVec4& v) const noexcept {
        return IntVec4(*this) += v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::operator + (s32 scalar) const noexcept {
        return IntVec4(*this) += scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::operator - (const IntVec4& v) const noexcept {
        return IntVec4(*this) -= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::operator - (s32 scalar) const noexcept {
        return IntVec4(*this) -= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::operator * (const IntVec4& v) const noexcept {
        return IntVec4(*this) *= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::operator * (s32 scalar) const noexcept {
        return IntVec4(*this) *= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::operator / (const IntVec4& v) const noexcept {
        return IntVec4(*this) /= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::operator / (s32 scalar) const noexcept {
        return IntVec4(*this) /= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec4& IntVec4::operator += (const IntVec4& v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec4& IntVec4::operator += (s32 scalar) noexcept {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子 
    //@―---------------------------------------------------------------------------
    constexpr IntVec4& IntVec4::operator -= (const IntVec4& v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec4& IntVec4::operator -= (s32 scalar) noexcept {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子 
    //@―---------------------------------------------------------------------------
    constexpr IntVec4& IntVec4::operator *= (const IntVec4& v) noexcept {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子 (スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec4& IntVec4::operator *= (s32 scalar) noexcept {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子 
    //@―---------------------------------------------------------------------------
    constexpr IntVec4& IntVec4::operator /= (const IntVec4& v) noexcept {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    constexpr IntVec4& IntVec4::operator /= (s32 scalar) noexcept {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 単項マイナス演算子
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::operator - () const noexcept {
        return IntVec4(-x, -y, -z, -w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      要素取得
    //! 
    //! @details    範囲外の場合は0を返します。  
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec4::operator [](size_t index) const noexcept {
        switch (index) {
        case 0:return x;
        case 1:return y;
        case 2:return z;
        case 3:return w;
        default:return 0;
        }
    }


    //===============================================================
    //  ゲッター
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 要素の最大を取得
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec4::maxComponent() const noexcept {
        return  std::max(std::max(x, y), std::max(z, w));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の絶対値の最大を取得
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec4::maxAbsComponent() const noexcept {
        return std::max(std::max(abs(x), abs(y)), std::max(abs(z), abs(w)));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の最小を取得
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec4::minComponent() const noexcept {
        return std::min(std::min(x, y), std::min(z, w));
    }


    //===============================================================
    //  判定
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief すべての要素が同じ値か判定
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec4::allComponentsEqual()const noexcept {
        return x == y && y == z && z == w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ゼロベクトルか判定
    //@―---------------------------------------------------------------------------
    constexpr bool IntVec4::isZero()const noexcept {
        return x == 0 && y == 0 && z == 0 && w == 0;
    }


    //===============================================================
    //  スタティック関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの各要素の絶対値からベクトルを作成
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::Abs(const IntVec4& v) noexcept {
        return IntVec4(Math::Abs(v.x), Math::Abs(v.y), Math::Abs(v.z), Math::Abs(v.w));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 内積
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec4::Dot(const IntVec4& a, const IntVec4& b) noexcept {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 外積
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::Cross(const IntVec4& a, const IntVec4& b) noexcept {
        return IntVec4(a.y * b.z - a.z * b.y, a.z * b.w - a.w * b.z, a.w * b.x - a.x * b.w, a.x * b.y - a.y * b.x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  二つのベクトルのマンハッタン距離を計算
    //@―---------------------------------------------------------------------------
    constexpr s32 IntVec4::MDist(const IntVec4& a, const IntVec4& b) noexcept {
        return
            Math::Abs(a.x - b.x) +
            Math::Abs(a.y - b.y) +
            Math::Abs(a.z - b.z) +
            Math::Abs(a.w - b.w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::Max(const IntVec4& a, const IntVec4& b) noexcept {
        return IntVec4(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
    //@―---------------------------------------------------------------------------
    constexpr IntVec4 IntVec4::Min(const IntVec4& a, const IntVec4& b) noexcept {
        return IntVec4(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
    }

    //! @endcond
}


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::IntVec4, ob::core::Char> : fmt::formatter<ob::core::s32, ob::core::Char> {
    using base = fmt::formatter<ob::core::s32, ob::core::Char>;
    template<typename FormatContext>
    auto format(const ob::core::IntVec4& value, FormatContext& ctx) -> decltype(ctx.out()) {
        ctx.advance_to(format_to(ctx.out(), TC("(")));
        ctx.advance_to(base::format(value.x, ctx));
        ctx.advance_to(format_to(ctx.out(), TC(",")));
        ctx.advance_to(base::format(value.y, ctx));
        ctx.advance_to(format_to(ctx.out(), TC(",")));
        ctx.advance_to(base::format(value.z, ctx));
        ctx.advance_to(format_to(ctx.out(), TC(",")));
        ctx.advance_to(base::format(value.w, ctx));
        ctx.advance_to(format_to(ctx.out(), TC(")")));
        return ctx.out();
    }
};
//! @endcond
