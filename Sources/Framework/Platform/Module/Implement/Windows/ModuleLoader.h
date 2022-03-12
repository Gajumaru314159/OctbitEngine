﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#ifdef OS_WINDOWS
#include "../IModuleLoader.h"
#include <Windows.h>

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class ModuleLoader :public IModuleLoader {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        ModuleLoader(StringView filePath);


        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        ~ModuleLoader();


        //@―---------------------------------------------------------------------------
        //! @brief  モジュール名を取得
        //@―---------------------------------------------------------------------------
        virtual StringView name()const { return m_name; };


        //@―---------------------------------------------------------------------------
        //! @brief  モジュールを取得
        //@―---------------------------------------------------------------------------
        IModule* getInterface()override {
            return m_interface.get();
        }

    private:

        String m_name;
        HMODULE m_handle;
        std::unique_ptr<IModule> m_interface;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespace ob::platform
#endif// OS_WINDOWS