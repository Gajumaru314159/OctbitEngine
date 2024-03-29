﻿//***********************************************************
//! @file
//! @brief		モジュール・マネージャ
//! @author		Gajumaru
//***********************************************************
#include "ModuleManager.h"

#include "Implement/Windows/ModuleLoader.h"

namespace ob::platform {



    //@―---------------------------------------------------------------------------
    //! @brief  モジュールを読み込み
    //@―---------------------------------------------------------------------------
    void ModuleManager::unloadModule(StringView moduleName) {
        auto found = m_moduleMap.find(moduleName);
        if (found != m_moduleMap.end()) {
            //found->second.release();
            m_moduleMap.erase(found);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  モジュールを読み込み
    //@―---------------------------------------------------------------------------
    IModule* ModuleManager::loadModuleImpl(StringView moduleName) {
        ModuleLoader module(moduleName);
        return nullptr;
        //auto moduleLoader = std::make_unique<ModuleLoader>(moduleName);
        //if (auto pModule = moduleLoader->getInterface()) {
        //    m_moduleMap[moduleName] = ob::move(moduleLoader);
        //} else {
        //    return nullptr;
        //}
        //
        //return m_moduleMap[moduleName]->getInterface();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  モジュールを取得
    //@―---------------------------------------------------------------------------
    IModule* ModuleManager::findModule(StringView moduleName)const {
        auto found = m_moduleMap.find(moduleName);
        if (found != m_moduleMap.end()) {
            if (found->second) {
                return found->second->getInterface();
            }
        }
        return nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  モジュールが読み込まれているか
    //@―---------------------------------------------------------------------------
    bool ModuleManager::hasLoadedModule(StringView moduleName)const {
        return findModule(moduleName)!=nullptr;
    }

}// namespace ob::platform