﻿//***********************************************************
//! @file
//! @brief		マージン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector/Vector2.h>
#include <Framework/Core/Geometory/Rect.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  マージン
    //@―---------------------------------------------------------------------------
    struct OB_API Margin {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Margin() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(ゼロ初期化)
        //@―---------------------------------------------------------------------------
        explicit Margin(EForceInit);


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(要素指定)
        //@―---------------------------------------------------------------------------
        Margin(f32 left, f32 top, f32 right, f32 bottom);


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          等価演算子
        //@―---------------------------------------------------------------------------
        bool operator==(const Margin& another)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          否等価演算子
        //@―---------------------------------------------------------------------------
        bool operator!=(const Margin& another)const noexcept;


        //===============================================================
        // セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  要素を指定
        //@―---------------------------------------------------------------------------
        void set(f32 left, f32 top, f32 right, f32 bottom)noexcept;


        //===============================================================
        // ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      左上
        //@―---------------------------------------------------------------------------
        Vec2 tl()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      右上
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
        //! @brief      左右の余白の合計
        //@―---------------------------------------------------------------------------
        f32 horizontal()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      上下の余白の合計
        //@―---------------------------------------------------------------------------
        f32 vertical()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  親サイズからサイズを計算
        //@―---------------------------------------------------------------------------
        Vec2 size(const Vec2& parent)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  親矩形から Rect を計算
        //@―---------------------------------------------------------------------------
        Rect rect(const Rect& parent)const noexcept;


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
    //! @brief  コンストラクタ(ゼロ初期化)
    //@―---------------------------------------------------------------------------
    inline Margin::Margin(EForceInit) {
        set(0.0f, 0.0f, 0.0f, 0.0f);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(要素指定)
    //@―---------------------------------------------------------------------------
    inline Margin::Margin(f32 left, f32 top, f32 right, f32 bottom)
        : left(left)
        , top(top)
        , right(right)
        , bottom(bottom) {
    }


    //@―---------------------------------------------------------------------------
    //! @brief          等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Margin::operator==(const Margin& another)const noexcept {
        return
            Mathf::IsNearEquals(left, another.left) &&
            Mathf::IsNearEquals(top, another.top) &&
            Mathf::IsNearEquals(right, another.right) &&
            Mathf::IsNearEquals(bottom, another.bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          否等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Margin::operator!=(const Margin& another)const noexcept {
        return !(*this == another);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  要素を指定
    //@―---------------------------------------------------------------------------
    inline void Margin::set(f32 left, f32 top, f32 right, f32 bottom)noexcept {
        this->left = left;
        this->top = top;
        this->right = right;
        this->bottom = bottom;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      左上
    //@―---------------------------------------------------------------------------
    inline Vec2 Margin::tl()const noexcept {
        return Vec2(left, top);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      右上
    //@―---------------------------------------------------------------------------
    inline Vec2 Margin::tr()const noexcept {
        return Vec2(right, top);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      左下
    //@―---------------------------------------------------------------------------
    inline Vec2 Margin::bl()const noexcept {
        return Vec2(left, bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      右下
    //@―---------------------------------------------------------------------------
    inline Vec2 Margin::br()const noexcept {
        return Vec2(right, bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      左右の余白の合計
    //@―---------------------------------------------------------------------------
    inline f32 Margin::horizontal()const noexcept {
        return left + right;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      上下の余白の合計
    //@―---------------------------------------------------------------------------
    inline f32 Margin::vertical()const noexcept {
        return top + bottom;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  親サイズからサイズを計算
    //@―---------------------------------------------------------------------------
    inline Vec2 Margin::size(const Vec2& parent)const noexcept {
        return Vec2(parent.x - left - right, parent.y - top - bottom);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  親矩形から Rect を計算
    //@―---------------------------------------------------------------------------
    inline Rect Margin::rect(const Rect& parent)const noexcept {
        return Rect(
            size(parent.size()),
            parent.center() + Vec2(left + right, top + bottom) * 0.5f
        );
    }


    //! @endcond
}// namespcae ob