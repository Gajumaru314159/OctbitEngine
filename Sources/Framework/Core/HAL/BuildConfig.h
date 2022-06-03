//***********************************************************
//! @file
//! @brief		ビルド設定
//! @author		Gajumaru
//***********************************************************
#pragma once
//! @cond
#include <Framework/Core/HAL/Platform.h>

//===============================================================
// ログ
//===============================================================

#define OB_ENABLE_LOG_FATAL
#define OB_ENABLE_LOG_ERROR
#define OB_ENABLE_LOG_WARNING
#define OB_ENABLE_LOG_INFO


//===============================================================
// アサーション
//===============================================================

#define OB_ENABLE_REQUIRE
#define OB_ENABLE_ENSURE


//===============================================================
// デバッグ
//===============================================================

#define OB_DEBUG_MUTEX
#define OB_DEBUG_SPIN_LOCK

//===============================================================
// プラットフォーム毎の定義で置き換え
//===============================================================
#if defined(OS_WINDOWS)
#include "Windows/BuildConfig.h"

#elif defined(OS_LINUX)
#include "Linux/BuildConfig.h"

#else
#pragma error("Config("OS_NAME")がありません")
#endif
//! @endcond