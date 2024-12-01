//***********************************************************
//! @file
//! @brief		動的ライブラリ
//! @author		Gajumaru
//***********************************************************
#ifdef OS_WINDOWS

#include <Framework/Platform/DLL.h>
#include <Framework/Core/Platform/WindowsHeaders.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/File/Path.h>


namespace ob::platform {

    //! @brief      コンストラクタ
    //! 
    //! @details    読み込むパスの拡張子はぷらっとふぉーむ
    //! @path       読み込む動的ライブラリのパス
    DLL::DLL(StringView path) {

        auto dllpath = Path::ReplaceExtension(path,"dll");
        
        WString wpath;
        StringEncoder::Encode(dllpath, wpath);
        HMODULE dll = ::LoadLibraryW(wpath.c_str());

        if (dll == nullptr) {
            LOG_ERROR_EX("System", "DLL[{0}]が見つかりませんでした。", dllpath);
            return;
        }

        m_path = dllpath;
        m_handle = dll;
    }

    //! @brief  デストラクタ
    DLL::~DLL() {
        if (m_handle) {
            ::FreeLibrary((HMODULE)m_handle);
        }
    }

    //! @brief  関数取得
    DLL::Function DLL::getFunction(StringView name) {
    
        if (m_handle == nullptr) {
            LOG_WARNING("関数取得に失敗。DLLが空です。");
            return {};
        }

        StringBase<char> nameANSI;
        StringEncoder::Encode(name, nameANSI);

        FARPROC proc = GetProcAddress((HMODULE)m_handle, nameANSI.c_str());
        if (proc == nullptr) {
            LOG_ERROR_EX("System", "DLLに関数{}が含まれていません。", name);
            return {};
        }

        Function function;
        function.m_ptr = proc;
        return function;
    }

    //! @brief      読み込み中の動的ライブラリのパスを取得
    const String& DLL::getPath()const {
        return m_path;
    }

}

#endif