﻿//***********************************************************
//! @file
//! @brief		モジュール読み込みインターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "../IModule.h"

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  モジュール読み込みインターフェイス
    //@―---------------------------------------------------------------------------
    class IModuleLoader {
    public:

        virtual ~IModuleLoader() = default;

        //@―---------------------------------------------------------------------------
        //! @brief  モジュール名を取得
        //@―---------------------------------------------------------------------------
        virtual StringView name()const=0;


        //@―---------------------------------------------------------------------------
        //! @brief  モジュール・インターフェイスを取得
        //@―---------------------------------------------------------------------------
        virtual IModule* getInterface() = 0;

    };

}// namespace ob::platform