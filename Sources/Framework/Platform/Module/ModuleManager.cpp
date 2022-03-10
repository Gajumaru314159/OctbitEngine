//***********************************************************
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
    void ModuleManager::UnloadModule(StringView moduleName) {
        auto found = m_moduleMap.find(moduleName);
        if (found != m_moduleMap.end()) {
            //found->second.release();
            m_moduleMap.erase(found);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  モジュールを読み込み
    //@―---------------------------------------------------------------------------
    IModule* ModuleManager::LoadModuleImpl(StringView moduleName) {
        ModuleLoader module(moduleName);
        return nullptr;
        //auto moduleLoader = std::make_unique<ModuleLoader>(moduleName);
        //if (auto pModule = moduleLoader->GetInterface()) {
        //    m_moduleMap[moduleName] = ob::move(moduleLoader);
        //} else {
        //    return nullptr;
        //}
        //
        //return m_moduleMap[moduleName]->GetInterface();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  モジュールを取得
    //@―---------------------------------------------------------------------------
    IModule* ModuleManager::GetModule(StringView moduleName)const {
        auto found = m_moduleMap.find(moduleName);
        if (found != m_moduleMap.end()) {
            if (found->second) {
                return found->second->GetInterface();
            }
        }
        return nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  モジュールが読み込まれているか
    //@―---------------------------------------------------------------------------
    bool ModuleManager::HasLoadedModule(StringView moduleName)const {
        return GetModule(moduleName)!=nullptr;
    }

}// namespace ob::platform