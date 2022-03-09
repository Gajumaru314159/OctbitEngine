//***********************************************************
//! @file
//! @brief		モジュール・マネージャ
//! @author		Gajumaru
//***********************************************************
#include "ModuleManager.h"

#include "Implement/Windows/ModuleLoader.h"

namespace ob::platform {



    //@―---------------------------------------------------------------------------
    //! @brief  モジュールの破棄
    //@―---------------------------------------------------------------------------
    void ModuleManager::UnloadModule(const String& moduleName) {


    }

    IModule* ModuleManager::LoadModuleImpl(const String& moduleName) {

        auto moduleLoader = std::make_unique<ModuleLoader>(moduleName);
        if (auto pModule = moduleLoader->GetInterface<IModule>()) {
            m_moduleMap[moduleName] = ob::move(moduleLoader);
        } else {
            return nullptr;
        }

        return m_moduleMap[moduleName]->GetInterface<IModule>();
    }

    //@―---------------------------------------------------------------------------
    //! @brief  モジュールの破棄
    //@―---------------------------------------------------------------------------
    //void ModuleManager::UnloadModule(gsl::not_null<IModule> pModule){
    //
    //}



    IModule* ModuleManager::GetModule(const String& moduleName) {
        return nullptr;
    }
    bool ModuleManager::HasLoadedModule(const String& moduleName)const {
        return false;////m_moduleList.find(TC("モジュール名")) != m_modeluMap.end();
    }

}// namespace ob::platform