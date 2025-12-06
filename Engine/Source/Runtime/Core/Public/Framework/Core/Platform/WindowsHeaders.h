//***********************************************************
//! @file
//! @author		Gajumaru
//! @note		このヘッダーはプラットフォーム固有機能なのでPublicなヘッダーではインクルードしないでください。
//***********************************************************
#pragma once
#include <Framework/Core/HAL/Platform.h>

#ifdef OS_WINDOWS
#define WIN32_LEAN_AND_MEAN     // ヘッダーからあまり使われない関数を省く
#include <Windows.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
#endif