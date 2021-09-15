//***********************************************************
//! @file
//! @brief		モジュール・マネージャ
//! @author		Gajumaru
//***********************************************************
#include "ModuleManager.h"
#include <windows.h>

namespace ob
{

	//-----------------------------------------------------------


    bool ModuleManager::AddModule(const char* pModuleName) {
        HMODULE dll=LoadLibrary(pModuleName);
        if (dll == nullptr) {
            LOG_ERROR_EX(TEXT("System"), TEXT("ファイルが見つかりませんでした。"));
            return false;
        }

        FARPROC proc = GetProcAddress(dll, "CreateModule");
        if (proc == nullptr) {
            LOG_ERROR_EX(TEXT("System"), TEXT("DLLにCreateModule()が含まれていません。"));
            return false;
        }

        typedef IModule* (*Func)();
        Func createModule = reinterpret_cast<Func>(proc);

        auto module = unique_ptr<IModule>(createModule());
        if (!module) {
            LOG_ERROR_EX(TEXT("System"), TEXT("モジュールの生成に失敗しました。"));
            return false;
        }

        m_modeluMap[TEXT("モジュール名")] = ob::move(module);
    }
    IModule* ModuleManager::GetModule(const char* pModuleName) {

    }
    bool ModuleManager::HasLoadedModule(const char* pModuleName)const {
        return m_modeluMap.find(TEXT("モジュール名")) != m_modeluMap.end();
    }
    IModule* ModuleManager::LoadModule(const char* pModuleName) {

    }
}// namespace ob