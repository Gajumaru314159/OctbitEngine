﻿//***********************************************************
//! @file
//! @brief		アサーション定義
//! @author		Gajumaru
//! 
//! @details    OB_ASSERT や OB_ASSERT はリリース版でプログラムに含まれません。
//!             アサーションは主にプログラムの前提条件を表すために使用されます。
//!             エラー処理には適切に対処してエラーログを出力するか、例外を発生させてください。
//***********************************************************
#pragma once
#include <Framework/Core/Log/LogMacro.h>


//============================================
// アサーションマクロ
//============================================

//@―---------------------------------------------------------------------------
//! @brief      アサーションの基底マクロ
//@―---------------------------------------------------------------------------
#define OB_ASSERT_BASE(expr,format,...)                             \
if(UNLIKELY(!(expr))){                                              \
    OB_LOG_BASE(ob::core::LogType::Fatal,"Assertion",format,__VA_ARGS__); \
}

//@―---------------------------------------------------------------------------
//! @brief      式を評価してアサートを発生させる
//! 
//! @details    式がfalseである場合エラーログを出力しプログラムを停止する。
//@―---------------------------------------------------------------------------
#define OB_CHECK_ASSERT(expr,format,...)				OB_ASSERT_BASE(expr,format,__VA_ARGS__)
//! @copydoc OB_ASSERT_EX
#define OB_CHECK_ASSERT_EXPR(expr)					    OB_CHECK_ASSERT(expr,#expr)

//@―---------------------------------------------------------------------------
//! @brief      アサートを出力する
//! 
//! @details    エラーログを出力しプログラムを停止する。
//@―---------------------------------------------------------------------------
#define OB_ASSERT(format,...)				OB_CHECK_ASSERT(false,format,__VA_ARGS__)


//============================================
// 典型アサーション
//============================================

//@―---------------------------------------------------------------------------
//! @brief      OutOfRangeの検出マクロ
//! 
//! @details    min<value<max でない場合エラーログを出力しプログラムを停止する。
//@―---------------------------------------------------------------------------
#define OB_ASSERT_RANGE(value, minVal, maxVal)		            OB_CHECK_ASSERT(minVal <= value && value < maxVal,"Out of range.[{0},{1}) value = {2}",minVal,maxVal,value)


//@―---------------------------------------------------------------------------
//! @brief      到達不能コードをマークするためのマクロ
//!
//! @details    式が呼び出された場合、エラーログを出力しプログラムを停止する。
//@―---------------------------------------------------------------------------
#define OB_UNREACHABLE()                                        OB_ASSERT("Unreachable code.")


//@―---------------------------------------------------------------------------
//! @brief      未実装の機能をマークする
//! 
//! @details    式が呼び出された場合、Warningログを出力する。
//@―---------------------------------------------------------------------------
#define OB_NOTIMPLEMENTED()                                     OB_ASSERT("Not implemented code.")


//============================================
// 無効化
//============================================

#ifndef OB_ENABLE_REQUIRE
#undef OB_ASSERT_EX
#define OB_ASSERT_EX(expr,format,...) /* space */
#endif // OB_DISABLE_REQUIRE

#ifndef OB_ENABLE_ENSURE
#undef OB_ASSERT_EX
#define OB_ASSERT_EX(expr,format,...)  /* space */
#endif // OB_DISABLE_ENSURE