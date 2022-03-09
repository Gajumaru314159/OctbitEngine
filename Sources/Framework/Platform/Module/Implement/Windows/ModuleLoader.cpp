//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#ifdef OS_WINDOWS
#include "ModuleLoader.h"
#include <Framework/Foundation/String/StringEncoder.h>

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    ModuleLoader::ModuleLoader(const String& fileName) {
        m_handle = NULL;

        StringBase<wchar_t> fileNameW;
        StringEncoder::Encode(fileName, fileNameW);
        HMODULE dll = ::LoadLibrary(fileNameW.c_str());

        if (dll == nullptr) {
            LOG_ERROR_EX("System", "{0}が見つかりませんでした。", fileName.c_str());
            return;
        }

        FARPROC proc = GetProcAddress(dll, "GetModule");
        if (proc == nullptr) {
            LOG_ERROR_EX("System", "{0}にCreateModule()が含まれていません。", fileName.c_str());
            return;
        }

        typedef IModule* (*Func)();
        Func createModule = reinterpret_cast<Func>(proc);
        IModule* pModule = createModule();
        if (!pModule) {
            LOG_ERROR_EX("System", "モジュールの生成に失敗しました。({0})", fileName.c_str());
            return;
        }

        m_handle = dll;
        m_interface = std::unique_ptr<IModule>(pModule);
    }

    ModuleLoader::~ModuleLoader() {
        m_interface.release();
        ::FreeLibrary(m_handle);
    }

}// namespace ob::platform
#endif// OS_WINDOWS