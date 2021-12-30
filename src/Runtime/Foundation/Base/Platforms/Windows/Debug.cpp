//***********************************************************
//! @file
//! @brief		デバッグ機能
//! @author		Gajumaru
//***********************************************************
#include <Runtime/Foundation/Base/Platform.h>
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
    ::OutputDebugString(message);
    ::OutputDebugString(TEXT("\n"));
}


//@―---------------------------------------------------------------------------
//! @brief  ブレークポイントの呼び出し
//@―---------------------------------------------------------------------------
void CallBreakPoint() {
#ifdef _DEBUG
    if (::IsDebuggerPresent()) {
        _CrtDbgBreak();
    }
#endif
}

#endif// OS_WINDOWS