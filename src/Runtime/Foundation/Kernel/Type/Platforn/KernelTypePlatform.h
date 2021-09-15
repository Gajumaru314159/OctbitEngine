//***********************************************************
//! @file
//! @brief		カーネル・タイプ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Platform.h>

#if defined(OS_WINDOWS)
#include "Windows/KernelTypeImpl.h"

#elif defined(OS_ANDROID)
#include "Android/KernelTypeImpl.h"


#else
#pragma error(OS_NAME"にKernelTypeImplの定義がありません")
#endif