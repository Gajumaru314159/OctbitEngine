//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/HAL/Platform.h>
#include <Framework/Core/String/StringEncoder.h>
#ifdef OS_WINDOWS


#define WIN32_LEAN_AND_MEAN     // ヘッダーからあまり使われない関数を省く
#include <Windows.h>
#include <WinSock2.h>

#ifdef _DEBUG
#include <crtdbg.h> 
#endif

using namespace ob::core;

//! @brief  システムログ出力 
void OutputDebugLog(const char* message) {
    WString wmessage;
    StringEncoder::Encode(message, wmessage);
    ::OutputDebugStringW(wmessage.c_str());
    ::OutputDebugStringW(L"\n");
}

//! @brief  ブレークポイントを呼び出し
void ShowMessageBox(const char* message) {
    WString wmessage;
    StringEncoder::Encode(message, wmessage);
    ::MessageBoxW(nullptr, wmessage.c_str(), L"OctbitEngine", MB_OK);
}

#endif// OS_WINDOWS