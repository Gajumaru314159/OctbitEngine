﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once


namespace ob::engine {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class AppBase {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

    protected:

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        AppBase();

        virtual void onStarted() {}

    private:

        bool startup();
        void addRequierModuule();

    private:

        Pimpl<class Engine> m_engine;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob