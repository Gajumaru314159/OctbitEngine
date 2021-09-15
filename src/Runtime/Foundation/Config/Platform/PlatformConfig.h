//***********************************************************
//! @file
//! @brief		各プラットフォームのコンフィグ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Platform.h>

#if defined(OS_WINDOWS)
#include "Windows/Config.h"

#elif defined(OS_ANDROID)
#include "Android/Config.h"

#else
#pragma error(OS_NAME"ではConfigの定義がありません")
#endif