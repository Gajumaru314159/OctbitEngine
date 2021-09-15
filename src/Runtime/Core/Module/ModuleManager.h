//***********************************************************
//! @file
//! @brief		モジュール・マネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "IModule.h"

namespace ob {

    //-----------------------------------------------------------------------------


    class ModuleManager:public singleton<ModuleManager> {
    public:

        bool AddModule(const char* pModuleName);
        IModule* GetModule(const char* pModuleName);
        bool HasLoadedModule(const char* pModuleName)const;
        IModule* LoadModule(const char* pModuleName);


    private:

        ob::map<String, unique_ptr<IModule>> m_modeluMap;

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------------------------


}// namespcae ob