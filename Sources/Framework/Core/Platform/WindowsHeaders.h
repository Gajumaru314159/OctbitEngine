//***********************************************************
//! @file
//! @brief		Windowsヘッダー読み込み
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/HAL/Platform.h>

#ifdef OS_WINDOWS
#define WIN32_LEAN_AND_MEAN     // ヘッダーからあまり使われない関数を省く
#include <Windows.h>
#endif