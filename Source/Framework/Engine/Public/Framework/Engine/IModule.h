﻿//***********************************************************
//! @file
//! @brief		モジュール・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/String/Name.h>

namespace ob::engine {

    class Engine;

    //@―---------------------------------------------------------------------------
    //! @brief      モジュール・インターフェイス
    //! 
    //! @details    スタートアップに依存のあるシステムをエンジンに追加するために使用します。
    //!             モジュールはデフォルト設定で起動する Engine& のみを引数に取るコンストラクタを持つようにしてください。
    //!             依存するモジュールはデフォルト設定で追加されますが、生成オプションを持たせる場合は 
    //!             ---
    //!             ## イベント
    //!             |名前|説明|
    //!             |----|----|
    //!             |start|エディタの再生ボタンを押したときに呼ばれます。|
    //!             |update|ゲームの実行中毎フレーム呼ばれます。|
    //!             |stop|エディタの停止ボタンを押したときに呼ばれます。|
    //@―---------------------------------------------------------------------------
    class IModule {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      生成に成功したか
        //! 
        //! @details    モジュール生成後にfalseを返す場合、生成に失敗されたとみなされ即座に破棄されます。
        //@―---------------------------------------------------------------------------
        virtual bool isValid()const { return true; }


        //@―---------------------------------------------------------------------------
        //! @brief  ゲーム開始イベント
        //@―---------------------------------------------------------------------------
        virtual void start() {}

        //@―---------------------------------------------------------------------------
        //! @brief  ゲーム更新イベント
        //@―---------------------------------------------------------------------------
        virtual void update(){}

        //@―---------------------------------------------------------------------------
        //! @brief  ゲーム停止イベント
        //@―---------------------------------------------------------------------------
        virtual void stop() {}

    protected:

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        IModule(Engine& engine): m_engine(engine){}

    private:

        Engine& m_engine;

    };



    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob