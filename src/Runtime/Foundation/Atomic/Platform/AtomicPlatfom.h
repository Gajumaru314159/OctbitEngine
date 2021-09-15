//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Platform.h>

#if defined(OS_WINDOWS)
#include "Windows/AtomicImpl.h"

#elif defined(OS_ANDROID)
#include "Android/AtomicImpl.h"


#else
#pragma error(OS_NAME"ではAtomicImplの定義がありません")
#endif