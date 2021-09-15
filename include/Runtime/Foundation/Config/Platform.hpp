//***********************************************************
//! @file
//! @brief		プラットフォーム設定
//! @author		Gajumaru
//***********************************************************
#pragma once


// ============================================
// プラットフォーム
// ============================================

#if defined(_WIN64)

#   define OB_OS_WINDOWS
#   define OB_OS_64BIT
#   define OB_LITTLE_EDIAN
#   define OS_NAME "Win64"


#elif defined(_WIN32)

#   define OB_OS_WINDOWS
#   define OB_OS_32BIT
#   define OB_LITTLE_EDIAN
#   define OS_NAME "Win64"


#elif defined(__APPLE__)
// TODO Appleアーキテクチャの確認
#   define OB_OS_MAC
#   define OB_OS_64BIT
#   define OB_LITTLE_EDIAN
#   define OS_NAME "Mac"


#elif defined(__ANDROID__)
// TODO Androidアーキテクチャの確認
#   define OB_OS_ANDROID
#if defined(__aarch64__)
#   define OB_OS_64BIT
#elif
#   define OB_OS_32BIT
#endif
#   define OB_LITTLE_EDIAN
#   define OS_NAME "Android"


#else

#   pragma message("ハードウェアが指定されていません")
#   error


#endif