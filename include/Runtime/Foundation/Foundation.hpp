//***********************************************************
//! @file
//! @brief		基礎ライブラリ
//! @author		Gajumaru
//***********************************************************
#pragma once


// ============================================
// プラットフォーム
// ============================================
#include "Config/Platform.hpp"

#ifdef OB_OS_WINDOWS
#   pragma warning(push)
#   pragma warning(disable:4996)    // 非推奨の使用
#   pragma warning(disable:4530)    //  C++ 例外処理
#endif

#include <stdio.h>

#ifdef OB_OS_WINDOWS
#   pragma warning(pop)
#endif

#include 