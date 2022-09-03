﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>
#include <Framework/Platform/Type/Language.h>


namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class PlatformModule : public ob::engine::IModule {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        PlatformModule(engine::Engine& engine);


        //@―---------------------------------------------------------------------------
        //! @brief  プラットフォームのシステム言語を取得する
        //@―---------------------------------------------------------------------------
        Language getSystemLanguage()const;


    private:

        Pimpl<class PlatformModuleImpl> m_impl;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob