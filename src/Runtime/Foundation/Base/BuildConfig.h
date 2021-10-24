//***********************************************************
//! @file
//! @brief		ビルド設定
//! @author		Gajumaru
//***********************************************************
#pragma once
//! @cond
#include "Discrimination.h"

//===============================================================
// ビルドオプション
//===============================================================

#define OB_ENABLE_LOG_FATAL
#define OB_ENABLE_LOG_ERROR
#define OB_ENABLE_LOG_WARNING
#define OB_ENABLE_LOG_INFO


//===============================================================
// プラットフォーム毎の定義で置き換え
//===============================================================
#if defined(OS_WINDOWS)
#include "Platforms/Windows/BuildConfig.h"

#elif defined(OS_ANDROID)
#include "Platforms/Linux/BuildConfig.h"

#else
#pragma error(OS_NAME"ではConfigの定義がありません")
#endif
//! @endcond