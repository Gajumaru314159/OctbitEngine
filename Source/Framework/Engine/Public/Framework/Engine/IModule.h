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
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class IModule {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        IModule(Engine& engine) 
            : m_engine(engine)
        {
        }

        virtual void update(){}

    private:

        Engine& m_engine;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob