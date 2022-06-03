//***********************************************************
//! @file
//! @brief		デバッグ機能
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/HAL/Platform.h>
#ifdef OS_WINDOWS


#define WIN32_LEAN_AND_MEAN     // ヘッダーからあまり使われない関数を省く
#include <Windows.h>
#include <WinSock2.h>

#ifdef _DEBUG
#include <crtdbg.h> 
#endif


//@―---------------------------------------------------------------------------
//! @brief  システムログ出力 
//@―---------------------------------------------------------------------------
void OutputDebugLog(const wchar_t* message) {
    ::OutputDebugStringW(message);
    ::OutputDebugStringW(L"\n");
}


//@―---------------------------------------------------------------------------
//! @brief  ブレークポイントを呼び出し
//@―---------------------------------------------------------------------------
void CallBreakPoint() {
#ifdef _DEBUG
    if (::IsDebuggerPresent()) {
        _CrtDbgBreak();
    }
#endif
}


//@―---------------------------------------------------------------------------
//! @brief  ブレークポイントを呼び出し
//@―---------------------------------------------------------------------------
void ShowMessageBox(const wchar_t* pMessage) {
    ::MessageBoxW(NULL, pMessage, L"OctbitEngine", MB_OK);
}

#endif// OS_WINDOWS