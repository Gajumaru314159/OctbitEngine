//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#ifdef OS_WINDOWS

#include <Framework/Platform/DynamicLibrary.h>
#include <Framework/Core/Platform/WindowsHeaders.h>
#include <Framework/Core/String/StringEncoder.h>


namespace ob::platform
{

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //! 
    //! @details    読み込むパスの拡張子はぷらっとふぉーむ
    //! @path       読み込む動的ライブラリのパス
    //@―---------------------------------------------------------------------------
    DynamicLibrary::DynamicLibrary(const Path& path) {
        m_handle = nullptr;
        m_path = path / ".dll";
        
        StringBase<wchar_t> fileNameW;
        StringEncoder::Encode(m_path.c_str(), fileNameW);
        HMODULE dll = ::LoadLibrary(fileNameW.c_str());

        if (dll == nullptr) {
            String pathString;
            StringEncoder::Encode(fileNameW, pathString);
            LOG_ERROR_EX("System", "{0}が見つかりませんでした。", pathString);
            return;
        }

        m_handle = dll;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    DynamicLibrary::~DynamicLibrary() {
        if (m_handle) {
            ::FreeLibrary((HMODULE)m_handle);
        }
    }

    //@―---------------------------------------------------------------------------
    //! @brief  関数取得
    //@―---------------------------------------------------------------------------
    DynamicLibrary::Function DynamicLibrary::getFunction(StringView name) {
    
        if (m_handle == nullptr) {
            LOG_WARNING("関数取得に失敗。DynamicLibraryが空です。");
            return {};
        }


        StringBase<wchar_t> nameW;
        StringEncoder::Encode(name, nameW);

        FARPROC proc = GetProcAddress((HMODULE)m_handle, "GetModule");
        if (proc == nullptr) {
            LOG_ERROR_EX("System", "DynamicLibraryに関数{}が含まれていません。", name);
            return {};
        }

        Function function;
        function.m_ptr = proc;
        return function;
    }


}// namespace ob

#endif