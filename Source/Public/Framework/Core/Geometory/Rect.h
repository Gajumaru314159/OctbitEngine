﻿//***********************************************************
//! @file
//! @brief		矩形
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector2.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief      矩形
    //! 
    //! @details    矩形は内部的に中心座標とサイズで管理されます。
    //! @note       size が負の数の時は未定義動作です。
    //@―---------------------------------------------------------------------------
    struct OB_API Rect {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      デフォルトコンストラクタ( ゼロ初期化 )
        //@―---------------------------------------------------------------------------
        Rect() noexcept = default;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(要素指定)
        //@―---------------------------------------------------------------------------
        constexpr Rect(f32 left, f32 top, f32 right, f32 bottom) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(サイズ/中心指定)
        //@―---------------------------------------------------------------------------
        constexpr Rect(const Vec2& center, const Vec2& size = Vec2::Zero) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(点群指定)
        //! 
        //! @details    指定された点群をすべて含むボックスを構築する
        //@―---------------------------------------------------------------------------
        Rect(const gsl::span<Vec2>& points) noexcept;


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator==(const Rect& other)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      否等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator!=(const Rect& other)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          ポイントを含むように矩形を拡大する
        //! 
        //! @param point    ポイント
        //@―---------------------------------------------------------------------------
        constexpr Rect operator+(const Vec2& point)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          ポイントを含むように矩形を拡大する
        //! 
        //! @param point    ポイント
        //@―---------------------------------------------------------------------------
        constexpr Rect& operator+=(const Vec2& point) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          矩形の論理積
        //@―---------------------------------------------------------------------------
        constexpr Rect operator&(const Rect& rhs) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          矩形の論理積
        //@―---------------------------------------------------------------------------
        constexpr Rect& operator&=(const Rect& rhs) noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      構造体の初期化
        //@―---------------------------------------------------------------------------
        constexpr void reset() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      LTRB を設定
        //@―---------------------------------------------------------------------------
        constexpr void set(f32 left, f32 top, f32 right, f32 bottom) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      2点を指定して Rect を設定
        //@―---------------------------------------------------------------------------
        constexpr void set(const Vec2& start, const Vec2& end) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      中央座標を設定
        //@―---------------------------------------------------------------------------
        constexpr void setCenter(const Vec2& center) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      中央座標を設定
        //@―---------------------------------------------------------------------------
        constexpr void setSize(const Vec2& size) noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      左上
        //@―---------------------------------------------------------------------------
        constexpr Vec2 tl()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      左上
        //@―---------------------------------------------------------------------------
        constexpr Vec2 tr()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      左下
        //@―---------------------------------------------------------------------------
        constexpr Vec2 bl()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      右下
        //@―---------------------------------------------------------------------------
        constexpr Vec2 br()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      中央
        //@―---------------------------------------------------------------------------
        constexpr Vec2 center()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      上辺の中央
        //@―---------------------------------------------------------------------------
        constexpr Vec2 topCenter()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      下辺の中央
        //@―---------------------------------------------------------------------------
        constexpr Vec2 bottomCenter()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      左辺の中央
        //@―---------------------------------------------------------------------------
        constexpr Vec2 leftCenter()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      右辺の中央
        //@―---------------------------------------------------------------------------
        constexpr Vec2 rightCenter()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      中央
        //@―---------------------------------------------------------------------------
        constexpr Vec2 size()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      幅
        //@―---------------------------------------------------------------------------
        constexpr f32 width()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      高さ
        //@―---------------------------------------------------------------------------
        constexpr f32 height()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      面積
        //@―---------------------------------------------------------------------------
        constexpr f32 area()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      外周の長さ
        //@―---------------------------------------------------------------------------
        constexpr f32 perimeter()const noexcept;


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      サイズが0以下であるか
        //@―---------------------------------------------------------------------------
        constexpr bool empty()const noexcept;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      2点を指定して Rect を作成する
        //@―---------------------------------------------------------------------------
        static constexpr Rect FromTo(const Vec2& start, const Vec2& end) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      補間
        //! 
        //! @param a    開始
        //! @param b    終了
        //! @param t    補間係数
        //! @return     t=0のときa、t=1の時bを返す。
        //@―---------------------------------------------------------------------------
        static constexpr Rect Lerp(const Rect& a, const Rect& b, f32 t) noexcept;


    public:

        f32 left;   //!< 左座標
        f32 top;    //!< 上座標
        f32 right;  //!< 右座標
        f32 bottom; //!< 下座標

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(サイズ/中心指定)
    //@―---------------------------------------------------------------------------
    constexpr Rect::Rect(f32 left, f32 top, f32 right, f32 bottom) noexcept
        : left(left),top(top),right(right),bottom(bottom)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(サイズ/中心指定)
    //@―---------------------------------------------------------------------------
    constexpr Rect::Rect(const Vec2& size, const Vec2& center) noexcept
        : Rect(center.x - size.x * 0.5f, center.y - size.y * 0.5f, center.x + size.x * 0.5f, center.y* size.y * 0.5f)
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool Rect::operator==(const Rect& other)const noexcept {
        return
            left == other.left &&
            top == other.top &&
            right == other.right &&
            bottom == other.bottom;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      否等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool Rect::operator!=(const Rect& other)const noexcept {
        return !(*this==other);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          ポイントを含むように矩形を拡大する
    //! 
    //! @param point    ポイント
    //@―---------------------------------------------------------------------------
    constexpr Rect Rect::operator+(const Vec2& point)const noexcept {
        return Rect(*this) += point;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          ポイントを含むように矩形を拡大する
    //! 
    //! @param point    ポイント
    //@―---------------------------------------------------------------------------
    constexpr Rect& Rect::operator+=(const Vec2& point) noexcept {
        left = Math::Min(point.x, left);
        right = Math::Max(point.x, right);
        top = Math::Min(point.y, top);
        bottom = Math::Max(point.y, bottom);
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          矩形の論理積
    //@―---------------------------------------------------------------------------
    constexpr Rect Rect::operator&(const Rect& rhs) noexcept {
        return Rect(
            Math::Max(left, rhs.left),
            Math::Max(top, rhs.top),
            Math::Min(right, rhs.right),
            Math::Min(bottom, rhs.bottom)
        );
    }


    //@―---------------------------------------------------------------------------
    //! @brief          矩形の論理積
    //@―---------------------------------------------------------------------------
    constexpr Rect& Rect::operator&=(const Rect& rhs) noexcept {
        *this = rhs;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      構造体のゼロ初期化
    //@―---------------------------------------------------------------------------
    constexpr void Rect::reset() noexcept {
        set(0.0f, 0.0f, 0.0f, 0.0f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      LTRB を設定
    //@―---------------------------------------------------------------------------
    constexpr void Rect::set(f32 left, f32 top, f32 right, f32 bottom) noexcept {
        this->left = left;
        this->top = top;
        this->right = right;
        this->bottom = bottom;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      2点を指定して Rect を設定
    //@―---------------------------------------------------------------------------
    constexpr void Rect::set(const Vec2& center, const Vec2& size) noexcept {
        set(center.x - size.x * 0.5f, center.y - size.y * 0.5f, center.x + size.x * 0.5f, center.y * size.y * 0.5f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      中央座標を設定
    //@―---------------------------------------------------------------------------
    constexpr void Rect::setCenter(const Vec2& center) noexcept {
        auto dt = center - this->center();
        left += dt.x;
        right += dt.x;
        top += dt.y;
        bottom += dt.y;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      中央座標を設定
    //@―---------------------------------------------------------------------------
    constexpr void Rect::setSize(const Vec2& size) noexcept {
        auto dt = size - this->size();
        left -= dt.x * 0.5f;
        right += dt.x * 0.5f;
        top -= dt.y * 0.5f;
        bottom += dt.y * 0.5f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      左上
    //@―---------------------------------------------------------------------------
    constexpr Vec2 Rect::tl()const noexcept {
        return Vec2(left, top);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      右上
    //@―---------------------------------------------------------------------------
    constexpr Vec2 Rect::tr()const noexcept {
        return Vec2(right, top);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      左下
    //@―---------------------------------------------------------------------------
    constexpr Vec2 Rect::bl()const noexcept {
        return Vec2(left, bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      右下
    //@―---------------------------------------------------------------------------
    constexpr Vec2 Rect::br()const noexcept {
        return Vec2(right, bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      中央
    //@―---------------------------------------------------------------------------
    constexpr Vec2 Rect::center()const noexcept {
        return Vec2((left + right) * 0.5f, (top + bottom) * 0.5f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      上辺の中央
    //@―---------------------------------------------------------------------------
    constexpr Vec2 Rect::topCenter()const noexcept {
        return Vec2((left + right) * 0.5f, top);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      下辺の中央
    //@―---------------------------------------------------------------------------
    constexpr Vec2 Rect::bottomCenter()const noexcept {
        return Vec2((left + right) * 0.5f, bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      左辺の中央
    //@―---------------------------------------------------------------------------
    constexpr Vec2 Rect::leftCenter()const noexcept {
        return Vec2(left, (top + bottom) * 0.5f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      右辺の中央
    //@―---------------------------------------------------------------------------
    constexpr Vec2 Rect::rightCenter()const noexcept {
        return Vec2(right, (top + bottom) * 0.5f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      高さ
    //@―---------------------------------------------------------------------------
    constexpr Vec2 Rect::size()const noexcept {
        return Vec2(width(), height());
    }


    //@―---------------------------------------------------------------------------
    //! @brief      幅
    //@―---------------------------------------------------------------------------
    constexpr f32 Rect::width()const noexcept {
        return right - left;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      高さ
    //@―---------------------------------------------------------------------------
    constexpr f32 Rect::height()const noexcept {
        return bottom - top;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      面積
    //@―---------------------------------------------------------------------------
    constexpr f32 Rect::area()const noexcept {
        return width() * height();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      外周の長さ
    //@―---------------------------------------------------------------------------
    constexpr f32 Rect::perimeter()const noexcept {
        return (width() + height()) * 2;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      サイズが0以下であるか
    //@―---------------------------------------------------------------------------
    constexpr bool Rect::empty()const noexcept {
        return area() == Math::EPSILON;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      2点を指定して Rect を作成する
    //@―---------------------------------------------------------------------------
    constexpr Rect Rect::FromTo(const Vec2& start, const Vec2& end) noexcept {
        return Rect(start - end, (start + end) * 0.5f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      補間
    //! 
    //! @param a    開始
    //! @param b    終了
    //! @param t    補間係数
    //! @return     t=0のときa、t=1の時bを返す。
    //@―---------------------------------------------------------------------------
    constexpr Rect Lerp(const Rect& a, const Rect& b, f32 t) noexcept {
        return Rect(
            Math::Lerp(a.left, b.left, t),
            Math::Lerp(a.top, b.top, t),
            Math::Lerp(a.right, b.right, t),
            Math::Lerp(a.bottom, b.bottom, t)
        );
    }


    //! @endcond
}// namespcae ob