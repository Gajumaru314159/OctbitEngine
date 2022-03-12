//***********************************************************
//! @file
//! @brief		ビルド設定
//! @author		Gajumaru
//***********************************************************
#pragma once
//! @cond
#include "Platform.h"

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

#define OB_DISABLE_REQUIRE
#define OB_DISABLE_ENSURE


//===============================================================
// プラットフォーム毎の定義で置き換え
//===============================================================
#if defined(OS_WINDOWS)
#include "Platforms/Windows/BuildConfig.h"

#elif defined(OS_LINUX)
#include "Platforms/Linux/BuildConfig.h"

#else
#pragma error("Config("OS_NAME")がありません")
#endif
//! @endcond