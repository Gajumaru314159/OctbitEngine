//***********************************************************
//! @file
//! @brief		各プラットフォームのコンフィグ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Platform.h>

#if defined(OS_WINDOWS)
#include "Windows/SystemHeap.h"

#elif defined(OS_ANDROID)
#include "Android/SystemHeap.h"

#else
#pragma error(OS_NAME"ではSystemHeapの定義がありません")
#endif