﻿//***********************************************************
//! @file
//! @brief		矩形
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector/include.h>

namespace ob {

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
        //! @brief      デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Rect() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ( ゼロ初期化 )
        //@―---------------------------------------------------------------------------
        explicit Rect(EForceInit) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(要素指定)
        //@―---------------------------------------------------------------------------
        Rect(s32 left, s32 top, s32 right, s32 bottom) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ(サイズ/中心指定)
        //@―---------------------------------------------------------------------------
        Rect(const Vec2& center, const Vec2& size = Vec2::zero) noexcept;


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
        //! @brief      比較演算子
        //@―---------------------------------------------------------------------------
        bool operator==(const Rect& other)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          ポイントを含むように矩形を拡大する
        //! 
        //! @param point    ポイント
        //@―---------------------------------------------------------------------------
        Rect operator+(const Vec2& point)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          ポイントを含むように矩形を拡大する
        //! 
        //! @param point    ポイント
        //@―---------------------------------------------------------------------------
        Rect& operator+=(const Vec2& point) noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      構造体の初期化
        //@―---------------------------------------------------------------------------
        void reset() noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      LTRB を設定
        //@―---------------------------------------------------------------------------
        void set(s32 left, s32 top, s32 right, s32 bottom) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      2点を指定して Rect を設定
        //@―---------------------------------------------------------------------------
        void set(const Vec2& start, const Vec2& end) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      中央座標を設定
        //@―---------------------------------------------------------------------------
        void setCenter(const Vec2& center) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      中央座標を設定
        //@―---------------------------------------------------------------------------
        void setSize(const Vec2& size) noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      左上
        //@―---------------------------------------------------------------------------
        Vec2 tl()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      左上
        //@―---------------------------------------------------------------------------
        Vec2 tr()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      左下
        //@―---------------------------------------------------------------------------
        Vec2 bl()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      右下
        //@―---------------------------------------------------------------------------
        Vec2 br()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      中央
        //@―---------------------------------------------------------------------------
        Vec2 center()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      上辺の中央
        //@―---------------------------------------------------------------------------
        Vec2 topCenter()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      下辺の中央
        //@―---------------------------------------------------------------------------
        Vec2 bottomCenter()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      左辺の中央
        //@―---------------------------------------------------------------------------
        Vec2 leftCenter()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      右辺の中央
        //@―---------------------------------------------------------------------------
        Vec2 rightCenter()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      中央
        //@―---------------------------------------------------------------------------
        Vec2 size()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      幅
        //@―---------------------------------------------------------------------------
        f32 width()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      高さ
        //@―---------------------------------------------------------------------------
        f32 height()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      面積
        //@―---------------------------------------------------------------------------
        f32 area()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      外周の長さ
        //@―---------------------------------------------------------------------------
        f32 perimeter()const noexcept;


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      サイズが0以下であるか
        //@―---------------------------------------------------------------------------
        bool isEmpty()const noexcept;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      2点を指定して Rect を作成する
        //@―---------------------------------------------------------------------------
        static Rect Create(const Vec2& start, const Vec2& end) noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      補間
        //! 
        //! @param a    開始
        //! @param b    終了
        //! @param t    補間係数
        //! @return     t=0のときa、t=1の時bを返す。
        //@―---------------------------------------------------------------------------
        static Rect Lerp(const Rect& a, const Rect& b,f32 t) noexcept;


    public:

        f32 left;
        f32 top;
        f32 right;
        f32 bottom;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief      デフォルトコンストラクタ
    //@―---------------------------------------------------------------------------
    inline Rect::Rect() noexcept {
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ( ゼロ初期化 )
    //@―---------------------------------------------------------------------------
    inline Rect::Rect(EForceInit) noexcept {
        reset();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(サイズ/中心指定)
    //@―---------------------------------------------------------------------------
    inline Rect::Rect(s32 left, s32 top, s32 right, s32 bottom) noexcept {
        set(left, top, right,bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ(サイズ/中心指定)
    //@―---------------------------------------------------------------------------
    inline Rect::Rect(const Vec2& size, const Vec2& center) noexcept{
        set(size,center);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      比較演算子
    //@―---------------------------------------------------------------------------
    inline bool Rect::operator==(const Rect& other)const noexcept {
        return
            left == other.left &&
            top == other.top &&
            right == other.right &&
            bottom == other.bottom;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          ポイントを含むように矩形を拡大する
    //! 
    //! @param point    ポイント
    //@―---------------------------------------------------------------------------
    inline Rect Rect::operator+(const Vec2& point)const noexcept {
        return Rect(*this) += point;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          ポイントを含むように矩形を拡大する
    //! 
    //! @param point    ポイント
    //@―---------------------------------------------------------------------------
    inline Rect& Rect::operator+=(const Vec2& point) noexcept {
        left = get_min(point.x, left);
        right = get_max(point.x, right);
        top = get_min(point.y, top);
        bottom = get_max(point.y, bottom);
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      構造体のゼロ初期化
    //@―---------------------------------------------------------------------------
    inline void Rect::reset() noexcept {
        set(0.0f, 0.0f, 0.0f, 0.0f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      LTRB を設定
    //@―---------------------------------------------------------------------------
    void Rect::set(s32 left, s32 top, s32 right, s32 bottom) noexcept {
        this->left = left;
        this->top = top;
        this->right = right;
        this->bottom = bottom;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      2点を指定して Rect を設定
    //@―---------------------------------------------------------------------------
    void Rect::set(const Vec2& center, const Vec2& size) noexcept {
        set(center.x - size.x * 0.5f, center.y - size.y * 0.5f, center.x + size.x * 0.5f, center.y * size.y * 0.5f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      中央座標を設定
    //@―---------------------------------------------------------------------------
    void Rect::setCenter(const Vec2& center) noexcept {
        auto dt = center - this->center();
        left += dt.x;
        right += dt.x;
        top += dt.y;
        bottom += dt.y;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      中央座標を設定
    //@―---------------------------------------------------------------------------
    void Rect::setSize(const Vec2& size) noexcept {
        auto dt = size - this->size();
        left -= dt.x*0.5f;
        right += dt.x * 0.5f;
        top -= dt.y * 0.5f;
        bottom += dt.y * 0.5f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      左上
    //@―---------------------------------------------------------------------------
    inline Vec2 Rect::tl()const noexcept {
        return Vec2(left,top);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      右上
    //@―---------------------------------------------------------------------------
    inline Vec2 Rect::tr()const noexcept {
        return Vec2(right, top);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      左下
    //@―---------------------------------------------------------------------------
    inline Vec2 Rect::bl()const noexcept {
        return Vec2(left, bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      右下
    //@―---------------------------------------------------------------------------
    inline Vec2 Rect::br()const noexcept {
        return Vec2(right, bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      中央
    //@―---------------------------------------------------------------------------
    inline Vec2 Rect::center()const noexcept {
        return Vec2((left + right) * 0.5f, (top + bottom) * 0.5f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      上辺の中央
    //@―---------------------------------------------------------------------------
    inline Vec2 Rect::topCenter()const noexcept {
        return Vec2((left + right) * 0.5f, top);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      下辺の中央
    //@―---------------------------------------------------------------------------
    inline Vec2 Rect::bottomCenter()const noexcept {
        return Vec2((left + right) * 0.5f, bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      左辺の中央
    //@―---------------------------------------------------------------------------
    inline Vec2 Rect::leftCenter()const noexcept {
        return Vec2(left, (top + bottom) * 0.5f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      右辺の中央
    //@―---------------------------------------------------------------------------
    inline Vec2 Rect::rightCenter()const noexcept {
        return Vec2(right, (top + bottom) * 0.5f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      高さ
    //@―---------------------------------------------------------------------------
    inline Vec2 Rect::size()const noexcept {
        return Vec2(width(),height());
    }


    //@―---------------------------------------------------------------------------
    //! @brief      幅
    //@―---------------------------------------------------------------------------
    inline f32 Rect::width()const noexcept {
        return right - left;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      高さ
    //@―---------------------------------------------------------------------------
    inline f32 Rect::height()const noexcept {
        return bottom-top;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      面積
    //@―---------------------------------------------------------------------------
    inline f32 Rect::area()const noexcept {
        return width() * height();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      外周の長さ
    //@―---------------------------------------------------------------------------
    inline f32 Rect::perimeter()const noexcept {
        return (width() + height()) * 2;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      サイズが0以下であるか
    //@―---------------------------------------------------------------------------
    inline bool Rect::isEmpty()const noexcept {
        return area() == Mathf::EPSILON;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      2点を指定して Rect を作成する
    //@―---------------------------------------------------------------------------
    inline Rect Rect::Create(const Vec2& start, const Vec2& end) noexcept {
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
    inline Rect Lerp(const Rect& a, const Rect& b, f32 t) noexcept {
        return Rect(
            Mathf::Lerp(a.left, b.left, t),
            Mathf::Lerp(a.top, b.top, t),
            Mathf::Lerp(a.right, b.right, t),
            Mathf::Lerp(a.bottom, b.bottom, t)
        );
    }


    //! @endcond
}// namespcae ob