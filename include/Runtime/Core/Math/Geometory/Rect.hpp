﻿//***********************************************************
//! @file
//! @brief		矩形構造体
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Math/Vector/include.hpp>

namespace ob
{

    //-----------------------------------------------------------
    //! @brief 矩形構造体
    //-----------------------------------------------------------
    struct OB_API Rect
    {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        Rect();                                                     // デフォルトコンストラクタ
        Rect(const Vec2& center, const Vec2& size = Vec2::zero);    // コンストラクタ(引数指定)
        Rect(const Vec2* points, Size count);                       // コンストラクタ(点群指定)


        //===============================================================
        //  オペレータ
        //===============================================================
        bool    operator==(const Rect& other)const;                 // 比較演算子
        Rect    operator+(const Vec2& point)const;                  // 加算演算子(合成)
        Rect& operator+=(const Vec2& point);                        // 加算代入演算子(合成)


        //===============================================================
        //  セッター
        //===============================================================
        void Reset();                                               // 構造体の初期化
        void SetStartEnd(const Vec2& start, const Vec2& end);       // 2点を指定して Rect を設定


        //===============================================================
        //  ゲッター
        //===============================================================
        Vec2 GetStart()const;                                       // ボックスの起点となる頂点を取得
        Vec2 GetEnd()const;                                         // GetStart() と対角となる頂点を取得


        //===============================================================
        //  判定
        //===============================================================
        b32 IsEmpty()const;                                         // サイズが0以下であるか


    public:

        static Rect Create(const Vec2& start, const Vec2& end);     // 2点を指定して Box を作成する


    public:

        Vec2 center;
        Vec2 size;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------
    //! @brief      デフォルトコンストラクタ
    //-----------------------------------------------------------
    inline Rect::Rect()
    {
        Reset();
    }


    //-----------------------------------------------------------
    //! @brief      コンストラクタ(引数指定)
    //-----------------------------------------------------------
    inline Rect::Rect(const Vec2& size, const Vec2& center)
        : center(center)
        , size(Vec2::Abs(size))
    {}


    //-----------------------------------------------------------
    //! @brief      比較演算子
    //-----------------------------------------------------------
    bool Rect::operator==(const Rect& other)const
    {
        return (center == other.center) && (size == other.size);
    }


    //-----------------------------------------------------------
    //! @brief          加算演算子
    //! 
    //! @details        ポイントを含むように矩形を拡大する
    //! @param point    ポイント
    //-----------------------------------------------------------
    inline Rect Rect::operator+(const Vec2& point)const
    {
        return Rect(*this) += point;
    }


    //-----------------------------------------------------------
    //! @brief          加算演算子
    //! 
    //! @details        ポイントを含むように矩形を拡大する
    //! @param point    ポイント
    //-----------------------------------------------------------
    inline Rect& Rect::operator+=(const Vec2& point)
    {
        Vec2 min = center - size * 0.5f;
        Vec2 max = center + size * 0.5f;

        min = Vec2::Min(min, point);
        max = Vec2::Min(max, point);

        center = (min + max) * 0.5f;
        size = max - min;

        return *this;
    }


    //-----------------------------------------------------------
    //! @brief      構造体の初期化
    //-----------------------------------------------------------
    inline void Rect::Reset()
    {
        center.SetZero();
        size.SetZero();
    }


    //-----------------------------------------------------------
    //! @brief      2点を指定して Box を設定
    //-----------------------------------------------------------
    inline void Rect::SetStartEnd(const Vec2& start, const Vec2& end)
    {
        center = (start + end) * 0.5f;
        size = Vec2::Abs(start - end);
    }


    //-----------------------------------------------------------
    //! @brief      矩形の起点となる頂点を取得
    //! 
    //! @see        GetEnd()
    //-----------------------------------------------------------
    inline Vec2 Rect::GetStart()const
    {
        return size + center * 0.5f;
    }


    //-----------------------------------------------------------
    //! @brief      GetStart() と対角となる頂点を取得
    //! 
    //! @see        GetStart()
    //-----------------------------------------------------------
    inline Vec2 Rect::GetEnd()const
    {
        return size - center * 0.5f;
    }


    //-----------------------------------------------------------
    //! @brief      サイズが0以下であるか
    //-----------------------------------------------------------
    inline b32 Rect::IsEmpty()const
    {
        return size.IsZero();
    }


    //-----------------------------------------------------------
    //! @brief      2点を指定して Rect を作成する
    //-----------------------------------------------------------
    inline Rect Rect::Create(const Vec2& start, const Vec2& end)
    {
        return Rect(start - end, (start + end) * 0.5f);
    }


}// namespcae ob