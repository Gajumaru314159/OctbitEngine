//***********************************************************
//! @file
//! @brief		モジュール・マネージャ
//! @author		Gajumaru
//***********************************************************
#include "ModuleManager.h"
#include <windows.h>

namespace ob {
    namespace platform {

        IModule* ModuleManager::LoadModuleImpl(const String& moduleName) {
            StringBase<wchar_t> moduleNameW;
            StringEncoder::Encode(moduleName, moduleNameW);
            HMODULE dll = LoadLibraryW(moduleNameW.c_str());
            if (dll == nullptr) {
                LOG_ERROR_EX(TEXT("System"), TEXT("{0}が見つかりませんでした。"), moduleName);
                return nullptr;
            }

            FARPROC proc = GetProcAddress(dll, "GetModule");
            if (proc == nullptr) {
                LOG_ERROR_EX(TEXT("System"), TEXT("{0}にCreateModule()が含まれていません。"), moduleName);
                return nullptr;
            }

            typedef IModule* (*Func)();
            Func createModule = reinterpret_cast<Func>(proc);
            IModule* pModule = createModule();
            if (!pModule) {
                LOG_ERROR_EX(TEXT("System"), TEXT("モジュールの生成に失敗しました。({0})"), moduleName);
                return nullptr;
            }

            //m_moduleList.emplace_back(make_pair(HashedName(moduleName), unique_ptr<IModule>(pModule)));

            // 起動呼び出し
            pModule->Startup();
            return pModule;
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
            return false;////m_moduleList.find(TEXT("モジュール名")) != m_modeluMap.end();
        }

    }// namespace platform
}// namespace ob