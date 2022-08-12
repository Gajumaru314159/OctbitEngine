﻿//***********************************************************
//! @file
//! @brief		ハードウェア共通デファイン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/HAL/Platform.h>        // プラットフォーム識別
#include <Framework/Core/HAL/BuildConfig.h>     // コンフィグ読み込み
#include <Framework/Core/HAL/PrimitiveType.h>   // プリミティブ型
#include <Framework/Core/HAL/Macro.h>           // マクロ

namespace ob {
    using namespace ob::core;
}

//============================================
// DLLエクスポート/インポート
//============================================

#if defined(OB_BUILD_DLL)
#define OB_API //DLL_EXPORT
#elif defined(OB_USE_DLL)
#define OB_API //DLL_IMPORT
#else
#define OB_API /**/
#endif
//@―---------------------------------------------------------------------------
//! @def		OB_API
//! @brief		DLLエクスポート/インポートマクロ
//! @details	DLL出力 / DLL入力 / その他(LIB)
//@―---------------------------------------------------------------------------