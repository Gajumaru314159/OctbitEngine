﻿//***********************************************************
//! @file
//! @brief		トランジション
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Mathf.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  トランジション
    //! 
    //! @details    値を0から1、1から0補完する
    //@―---------------------------------------------------------------------------
    class Transition {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Transition() :time(0.0f) {};


        //@―---------------------------------------------------------------------------
        //! @brief  値を取得
        //@―---------------------------------------------------------------------------
        f32 get()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  更新
        //@―---------------------------------------------------------------------------
        f32 update(f32 deltaTime)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  要素を設定
        //@―---------------------------------------------------------------------------
        void set(f32 length, f32 in, f32 out, f32 minValue = 0.0f, f32 maxValue = 1.0f)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  再生位置を開始地点に戻す
        //@―---------------------------------------------------------------------------
        void restart()noexcept;


    private:

        f32 time;       //!< 現在の時間
        f32 length;     //!< 全体の長さ
        f32 in;         //!< フェードイン時間
        f32 out;        //!< フェードアウト時間
        f32 minValue;   //!< 最小値
        f32 maxValue;   //!< 最大値

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief  更新
    //@―---------------------------------------------------------------------------
    inline f32 Transition::update(f32 deltaTime)noexcept {
        time += deltaTime;
        return get();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  要素を設定
    //@―---------------------------------------------------------------------------
    inline void Transition::set(f32 length, f32 in, f32 out, f32 minValue, f32 maxValue)noexcept {
        this->length = length;
        this->in = in;
        this->out = out;
        this->minValue = minValue;
        this->maxValue = maxValue;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  再生位置を開始地点に戻す
    //@―---------------------------------------------------------------------------
    inline void Transition::restart()noexcept {
        time = 0.0f;
    }


    //! @endcond
}// namespcae ob