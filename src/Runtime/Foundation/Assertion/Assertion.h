//***********************************************************
//! @file
//! @brief		アサーション定義
//! @author		Gajumaru
//! 
//! @details    OB_REQUIRE や OB_ENSURE はリリース版でプログラムに含まれません。
//!             アサーションは主にプログラムの前提条件を表すために使用されます。
//!             エラー処理には適切に対処してエラーログを出力するか、例外を発生させてください。
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include <Foundation/Log/LogMacro.h>


//============================================
// アサーションマクロ
//============================================

//@―---------------------------------------------------------------------------
//! @brief      アサーションの基底マクロ
//@―---------------------------------------------------------------------------
#define OB_ASSERT_BASE(expr,level,format,...)                   \
if(UNLIKELY(!(expr))){                                          \
    OB_LOG_BASE(level,TEXT("Assertion"),format,__VA_ARGS__);    \
}


//@―---------------------------------------------------------------------------
//! @brief      関数やロジックを実行する前の前提条件を確認する
//! 
//! @details    式がfalseである場合エラーログを出力しプログラムを停止する。
//@―---------------------------------------------------------------------------
#define OB_REQUIRE_EX(expr,format,...)				OB_ASSERT_BASE(expr,ob::LogType::Fatal,format,__VA_ARGS__)
#define OB_REQUIRE(expr)					        OB_REQUIRE_EX(expr,TEXT(#expr))


//@―---------------------------------------------------------------------------
//! @brief      関数やロジックを実行した後の条件を確認する
//! 
//! @details    式がfalseである場合警告ログを出力する。
//@―---------------------------------------------------------------------------
#define OB_ENSURE_EX(expr,format,...)				OB_ASSERT_BASE(expr,ob::LogType::Warning,format,__VA_ARGS__)
#define OB_ENSURE(expr)					            OB_ENSURE_EX(expr,TEXT(#expr))


//============================================
// 典型アサーション
//============================================

//@―---------------------------------------------------------------------------
//! @brief      OutOfRangeの検出マクロ
//! 
//! @details    min<value<max でない場合エラーログを出力しプログラムを停止する。
//@―---------------------------------------------------------------------------
#define OB_REQUIRE_RANGE(value, minVal, maxVal)		            OB_REQUIRE(minVal <= value && value < maxVal,category,TEXT("Out of range."))


//@―---------------------------------------------------------------------------
//! @brief      到達不能コードをマークするためのマクロ
//!
//! @details    式が呼び出された場合、エラーログを出力しプログラムを停止する。
//@―---------------------------------------------------------------------------
#define OB_UNREACHABLE()                                        OB_REQUIRE(false,TEXT("Unreachable code."))


//@―---------------------------------------------------------------------------
//! @brief      未実装の機能をマークする
//! 
//! @details    式が呼び出された場合、Warningログを出力する。
//@―---------------------------------------------------------------------------
#define OB_NOTIMPLEMENTED()                                     OB_ENSURE(false,TEXT("Not implemented code."))


//============================================
// 無効化
//============================================

#ifdef OB_DISABLE_REQUIRE
#undef OB_REQUIRE_EX
#define OB_REQUIRE_EX(expr,format,...) /* space */
#endif // !OB_ENABLE_REQUIRE

#ifdef OB_DISABLE_ENSURE
#undef OB_ENSURE_EX
#define OB_ENSURE_EX(expr,format,...)  /* space */
#endif // !OB_ENABLE_ENSURE
