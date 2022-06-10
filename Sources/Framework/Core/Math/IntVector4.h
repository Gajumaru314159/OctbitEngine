﻿//***********************************************************
//! @file
//! @brief 整数四次元ベクトル
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
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
        //! @brief      コンストラクタ(ゼロ初期化)
        //@―---------------------------------------------------------------------------
        explicit IntVec4(EForceInit)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
        //@―---------------------------------------------------------------------------
        explicit IntVec4(s32 scalar)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(要素を指定して初期化)
        //@―---------------------------------------------------------------------------
        IntVec4(s32 x, s32 y, s32 z, s32 w)noexcept;


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
        bool operator == (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子
        //@―---------------------------------------------------------------------------
        bool operator != (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子
        //@―---------------------------------------------------------------------------
        IntVec4 operator + (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        IntVec4 operator + (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子
        //@―---------------------------------------------------------------------------
        IntVec4 operator - (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        IntVec4 operator - (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子
        //@―---------------------------------------------------------------------------
        IntVec4 operator * (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        IntVec4 operator * (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子
        //@―---------------------------------------------------------------------------
        IntVec4 operator / (const IntVec4& v) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算演算子(スカラー)
        //@―---------------------------------------------------------------------------
        IntVec4 operator / (s32 scalar) const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子
        //@―---------------------------------------------------------------------------
        IntVec4& operator += (const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 加算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        IntVec4& operator += (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子 
        //@―---------------------------------------------------------------------------
        IntVec4& operator -= (const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 減算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        IntVec4& operator -= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子 
        //@―---------------------------------------------------------------------------
        IntVec4& operator *= (const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 乗算代入演算子 (スカラー)
        //@―---------------------------------------------------------------------------
        IntVec4& operator *= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子 
        //@―---------------------------------------------------------------------------
        IntVec4& operator /= (const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 除算代入演算子(スカラー)
        //@―---------------------------------------------------------------------------
        IntVec4& operator /= (s32 scalar) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 単項マイナス演算子
        //@―---------------------------------------------------------------------------
        IntVec4 operator - () const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief s32*IntVec4演算子 
        //@―---------------------------------------------------------------------------
        friend IntVec4 operator * (s32 scalar, const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief s32/IntVec4演算子 
        //@―---------------------------------------------------------------------------
        friend IntVec4 operator / (s32 scalar, const IntVec4& v) noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 要素を設定
        //@―---------------------------------------------------------------------------
        void    set(s32 x, s32 y, s32 z, s32 w) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素を設定
        //@―---------------------------------------------------------------------------
        void    set(const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素を全て0に設定
        //@―---------------------------------------------------------------------------
        void    setZero() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 全ての要素を同じ値で設定
        //@―---------------------------------------------------------------------------
        void    setAll(s32 scalar) noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 要素の最大を取得
        //@―---------------------------------------------------------------------------
        s32     maxComponent() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素の絶対値の最大を取得
        //@―---------------------------------------------------------------------------
        s32     maxAbsComponent() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 要素の最小を取得
        //@―---------------------------------------------------------------------------
        s32     minComponent() const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief Vec4 で取得
        //@―---------------------------------------------------------------------------
        Vec4    toVec4() const noexcept;


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief すべての要素が同じ値か判定
        //@―---------------------------------------------------------------------------
        bool    allComponentsEqual()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief ゼロベクトルか判定
        //@―---------------------------------------------------------------------------
        bool    isZero()const noexcept;


    public:

        //===============================================================
        //  スタティック関数
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief ベクトルの各要素の絶対値からベクトルを作成
        //@―---------------------------------------------------------------------------
        static IntVec4  Abs(const IntVec4& v) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 内積
        //@―---------------------------------------------------------------------------
        static s32      Dot(const IntVec4& a, const IntVec4& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 外積
        //@―---------------------------------------------------------------------------
        static IntVec4  Cross(const IntVec4& a, const IntVec4& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  二つのベクトルのマンハッタン距離を計算
        //@―---------------------------------------------------------------------------
        static s32      MDist(const IntVec4& a, const IntVec4& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
        //@―---------------------------------------------------------------------------
        static IntVec4  Max(const IntVec4& a, const IntVec4& b) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
        //@―---------------------------------------------------------------------------
        static IntVec4  Min(const IntVec4& a, const IntVec4& b) noexcept;


    public:

        static const IntVec4 zero;    //!< ( 0, 0, 0, 0)
        static const IntVec4 one;     //!< ( 1, 1, 1, 1)

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
    inline IntVec4::IntVec4() noexcept {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(ゼロ初期化)
    //@―---------------------------------------------------------------------------
    inline IntVec4::IntVec4(EForceInit)noexcept {
        setZero();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(すべての要素を同じ値で初期化)
    //@―---------------------------------------------------------------------------
    inline IntVec4::IntVec4(s32 scalar) noexcept {
        set(scalar, scalar, scalar, scalar);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(要素を指定して初期化)
    //@―---------------------------------------------------------------------------
    inline IntVec4::IntVec4(s32 x, s32 y, s32 z, s32 w) noexcept {
        set(x, y, z, w);
    }


    //===============================================================
    //  オペレータ
    //===============================================================


    //@―---------------------------------------------------------------------------
    //! @brief      等価演算子
    //@―---------------------------------------------------------------------------
    inline bool IntVec4::operator == (const IntVec4& v) const noexcept {
        return
            x == v.x &&
            y == v.y &&
            z == v.z &&
            w == v.w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    inline bool IntVec4::operator != (const IntVec4& v) const noexcept {
        return !(operator==(v));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::operator + (const IntVec4& v) const noexcept {
        return IntVec4(*this) += v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::operator + (s32 scalar) const noexcept {
        return IntVec4(*this) += scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::operator - (const IntVec4& v) const noexcept {
        return IntVec4(*this) -= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::operator - (s32 scalar) const noexcept {
        return IntVec4(*this) -= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::operator * (const IntVec4& v) const noexcept {
        return IntVec4(*this) *= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::operator * (s32 scalar) const noexcept {
        return IntVec4(*this) *= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::operator / (const IntVec4& v) const noexcept {
        return IntVec4(*this) /= v;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::operator / (s32 scalar) const noexcept {
        return IntVec4(*this) /= scalar;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子
    //@―---------------------------------------------------------------------------
    inline IntVec4& IntVec4::operator += (const IntVec4& v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 加算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline IntVec4& IntVec4::operator += (s32 scalar) noexcept {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子 
    //@―---------------------------------------------------------------------------
    inline IntVec4& IntVec4::operator -= (const IntVec4& v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 減算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline IntVec4& IntVec4::operator -= (s32 scalar) noexcept {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子 
    //@―---------------------------------------------------------------------------
    inline IntVec4& IntVec4::operator *= (const IntVec4& v) noexcept {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乗算代入演算子 (スカラー)
    //@―---------------------------------------------------------------------------
    inline IntVec4& IntVec4::operator *= (s32 scalar) noexcept {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子 
    //@―---------------------------------------------------------------------------
    inline IntVec4& IntVec4::operator /= (const IntVec4& v) noexcept {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 除算代入演算子(スカラー)
    //@―---------------------------------------------------------------------------
    inline IntVec4& IntVec4::operator /= (s32 scalar) noexcept {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 単項マイナス演算子
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::operator - () const noexcept {
        return IntVec4(-x, -y, -z, -w);
    }


    //===============================================================
    //  セッター
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 要素を設定
    //@―---------------------------------------------------------------------------
    inline void IntVec4::set(s32 x, s32 y, s32 z, s32 w) noexcept {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を設定
    //@―---------------------------------------------------------------------------
    inline void IntVec4::set(const IntVec4& v) noexcept {
        set(v.x, v.y, v.z, v.w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を全て0に設定
    //@―---------------------------------------------------------------------------
    inline void IntVec4::setZero() noexcept {
        set(0, 0, 0, 0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 全ての要素を同じ値で設定
    //@―---------------------------------------------------------------------------
    inline void IntVec4::setAll(s32 scalar) noexcept {
        set(scalar, scalar, scalar, scalar);
    }


    //===============================================================
    //  ゲッター
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief 要素の最大を取得
    //@―---------------------------------------------------------------------------
    inline s32 IntVec4::maxComponent() const noexcept {
        return  std::max(std::max(x, y), std::max(z, w));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の絶対値の最大を取得
    //@―---------------------------------------------------------------------------
    inline s32 IntVec4::maxAbsComponent() const noexcept {
        return std::max(std::max(abs(x), abs(y)), std::max(abs(z), abs(w)));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素の最小を取得
    //@―---------------------------------------------------------------------------
    inline s32 IntVec4::minComponent() const noexcept {
        return std::min(std::min(x, y), std::min(z, w));
    }


    //===============================================================
    //  判定
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief すべての要素が同じ値か判定
    //@―---------------------------------------------------------------------------
    inline bool IntVec4::allComponentsEqual()const noexcept {
        return x == y && y == z && z == w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ゼロベクトルか判定
    //@―---------------------------------------------------------------------------
    inline bool IntVec4::isZero()const noexcept {
        return x == 0 && y == 0 && z == 0 && w == 0;
    }


    //===============================================================
    //  スタティック関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief ベクトルの各要素の絶対値からベクトルを作成
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::Abs(const IntVec4& v) noexcept {
        return IntVec4(abs(v.x), abs(v.y), abs(v.z), abs(v.w));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 内積
    //@―---------------------------------------------------------------------------
    inline s32 IntVec4::Dot(const IntVec4& a, const IntVec4& b) noexcept {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 外積
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::Cross(const IntVec4& a, const IntVec4& b) noexcept {
        return IntVec4(a.y * b.z - a.z * b.y, a.z * b.w - a.w * b.z, a.w * b.x - a.x * b.w, a.x * b.y - a.y * b.x);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  二つのベクトルのマンハッタン距離を計算
    //@―---------------------------------------------------------------------------
    inline s32 IntVec4::MDist(const IntVec4& a, const IntVec4& b) noexcept {
        return
            abs(a.x - b.x) +
            abs(a.y - b.y) +
            abs(a.z - b.z) +
            abs(a.w - b.w);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの各要素の大きいほうからベクトルを生成
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::Max(const IntVec4& a, const IntVec4& b) noexcept {
        return IntVec4(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 二つのベクトルの各要素の小さいほうからベクトルを生成
    //@―---------------------------------------------------------------------------
    inline IntVec4 IntVec4::Min(const IntVec4& a, const IntVec4& b) noexcept {
        return IntVec4(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
    }

    //! @endcond
}// namespace ob::core


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::IntVec4, ob::core::Char> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template<typename FormatContext>
    auto format(ob::core::IntVec4 value, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(), TC("({},{},{},{})"), value.x, value.y, value.z, value.w);
    }
};
//! @endcond