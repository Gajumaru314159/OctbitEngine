//***********************************************************
//! @file
//! @brief		各プラットフォームのCriticalSectionObject
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Platform.h>

#if defined(OS_WINDOWS)
#include "Windows/CriticalSectionObjectImpl.h"

#elif defined(OS_ANDROID)
#include "Android/CriticalSectionObjectImpl.h"

#else
#pragma error(OS_NAME"ではCriticalSectionObjectImplの定義がありません")
#endif