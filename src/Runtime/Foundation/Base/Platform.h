//***********************************************************
//! @file
//! @brief		プラットフォーム設定
//! @author		Gajumaru
//***********************************************************
#pragma once

//! @cond

// ============================================
// プラットフォーム
// ============================================

#if defined(_WIN64)
#   define OS_WINDOWS
#   define OS_64BIT
#   define LITTLE_EDIAN
#   define OS_NAME "Win64"


#elif defined(_WIN32)
#   define OS_WINDOWS
#   define OS_32BIT
#   define LITTLE_EDIAN
#   define OS_NAME "Win64"


#elif defined(__APPLE__)
// TODO Appleアーキテクチャの確認
#   define OS_MAC
#   define OS_64BIT
#   define LITTLE_EDIAN
#   define OS_NAME "Mac"


#elif defined(__ANDROID__)
// TODO Androidアーキテクチャの確認
#   define OS_ANDROID
#if defined(__aarch64__)
#   define OS_64BIT
#elif
#   define OS_32BIT
#endif
#   define LITTLE_EDIAN
#   define OS_NAME "Android"


#else
// 未実装
#   pragma message("ハードウェアが指定されていません")
#   error


#endif

//! @endcond