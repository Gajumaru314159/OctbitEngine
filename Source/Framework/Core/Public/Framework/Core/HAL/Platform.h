//***********************************************************
//! @file
//! @brief		プラットフォーム識別
//! @author		Gajumaru
//***********************************************************
#pragma once

//! @cond

//============================================
// OS識別
//============================================

// Windows(64bit)
#if defined(_WIN64)
#define OS_WINDOWS
#define OS_64BIT
#define OS_NAME "Windows(64bit)"


// Windows(32bit)
#elif defined(_WIN32)
#define OS_WINDOWS
#define OS_32BIT
#define OS_NAME "Windows(32bit)"


// Linux(64bit)
#elif (defined(__linux) || defined(__linux__)) and defined(__x86_64)
#define OS_LINUX
#define OS_32BIT
#define OS_NAME "Linux(64bit)"


// Linux(64bit)
#elif (defined(__linux) || defined(__linux__))
#define OS_LINUX
#define OS_64BIT
#define OS_NAME "Linux(32bit)"


#else
#pragma message("このプラットフォームには対応していません。")
#pragma message("対応プラットフォーム　:Windows(64bit/32bit)")
#pragma message("　　　　　　　　　　　:Linux(64bit/32bit)")
#error
// その他OS
//
// * Android
// * maxOS / iOS
// * 各種コンシューマ向けゲーム機
#endif



//============================================
// コンパイラ判定
//============================================

// Visual C++(VC2017以降)
#if defined(_MSC_VER) && (1910<=_MSC_VER)
#define COMPILER_MSVC


// GCC
#elif defined(__GNUC__)
#define COMPILER_GCC


#else
#pragma message("このコンパイラには対応していません。")
#pragma message("対応コンパイラ　:MSVC(VC2017～)")
#pragma message("　　　　　　　　:GCC")
#error
// その他コンパイラ
// 
// * IAR EWARM
// * ARM
// * インテル
// * Borland C++
// * Apple
#endif



//============================================
// エンディアン
//============================================



//============================================
// デバッグ判定
//============================================

#if defined(_DEBUG)||defined(DEBUG)
#define OB_DEBUG 1
#else
#define OB_DEBUG 0
#endif



//============================================
// 固有文脈
//============================================
// OS_WINDOWS
#ifdef OS_WINDOWS
#	define OS_WINDOWS_CONTEXT(...)		__VA_ARGS__
#else
#	define OS_WINDOWS_CONTEXT(...)		/**/
#endif

// OS_LINUX
#ifdef OS_LINUX
#	define OS_LINUX_CONTEXT(...)		__VA_ARGS__
#else
#	define OS_LINUX_CONTEXT(...)		/**/
#endif

// OB_DEBUG
#ifdef OB_DEBUG
#	define OB_DEBUG_CONTEXT(...)		__VA_ARGS__
#else
#	define OB_DEBUG_CONTEXT(...)		/**/
#endif

//! @endcond
