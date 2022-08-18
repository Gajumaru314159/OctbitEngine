//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#ifdef OS_WINDOWS
#include "ModuleLoader.h"
#include <Framework/Core/String/StringEncoder.h>

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    ModuleLoader::ModuleLoader(StringView fileName) {
        m_handle = NULL;
        m_name = fileName;

        IModule* pModule = nullptr;
        StringBase<wchar_t> fileNameW;
        StringEncoder::Encode(m_name+TC(".dll"), fileNameW);
        HMODULE dll = ::LoadLibrary(fileNameW.c_str());

        if (dll == nullptr) {
            LOG_ERROR_EX("System", "{0}が見つかりませんでした。", m_name.c_str());
            return;
        }
        {
            FARPROC proc = GetProcAddress(dll, "GetModule");
            if (proc == nullptr) {
                LOG_ERROR_EX("System", "{0}にCreateModule()が含まれていません。", m_name.c_str());
                return;
            }

            typedef IModule* (*Func)();
            Func createModule = reinterpret_cast<Func>(proc);
            pModule = createModule();
            if (!pModule) {
                LOG_ERROR_EX("System", "モジュールの生成に失敗しました。({0})", m_name.c_str());
                return;
            }
        }

        m_handle = dll;

        // pModuleはstatic変数
        m_pModule = pModule;
    }

    ModuleLoader::~ModuleLoader() {
        if (m_pModule) {
            m_pModule->shutdown();
            // モジュールはstatic変数なので削除しない
        }
        ::FreeLibrary(m_handle);
    }

}// namespace ob::platform
#endif// OS_WINDOWS