﻿//***********************************************************
//! @file
//! @brief		ログ出力マクロ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Logger.h"

//============================================
// ログ出力マクロ
//============================================

//@―---------------------------------------------------------------------------
//! @brief      デフォルトカテゴリ名
//@―---------------------------------------------------------------------------
#define LOG_DEFAULT_CATEGORY "Default"

//@―---------------------------------------------------------------------------
//! @brief      基底のログ出力マクロ
//@―---------------------------------------------------------------------------
#define OB_LOG_BASE(type,category,format,...) ob::Logger::ref().addLog(type,CURRENT_SOURCE_LOCATION,TC(category),TC(format), __VA_ARGS__)


//@―---------------------------------------------------------------------------
//! @brief      アプリケーションの継続が難しい致命的なエラーを通知
//!
//! @details    アプリケーションは直ちに終了しなければならない。n
//!             ブレークポイントを発生させる。
//! @param category カテゴリ名
//! @param format   フォーマット文字列
//! @param args     フォーマット引数
//@―---------------------------------------------------------------------------
#define LOG_FATAL_EX(category,format,...)	    OB_LOG_BASE(ob::LogType::Fatal,category, format, __VA_ARGS__)
//@―---------------------------------------------------------------------------
//! @brief      アプリケーションの継続が難しい致命的なエラーを通知
//!
//! @details    アプリケーションは直ちに終了しなければならない。@n
//!             ブレークポイントを発生させる。//!             
//! @param format   フォーマット文字列
//! @param args     フォーマット引数
//@―---------------------------------------------------------------------------
#define LOG_FATAL(format,...)                   LOG_FATAL_EX(LOG_DEFAULT_CATEGORY,format,__VA_ARGS__)


//@―---------------------------------------------------------------------------
//! @brief      アプリケーションが不正な状態になる可能性がある問題を通知
//!
//! @details    アプリケーションの継続が危険であり、早急に終了することが望まれる。
//! @param category カテゴリ名
//! @param format   フォーマット文字列
//! @param args     フォーマット引数
//@―---------------------------------------------------------------------------
#define LOG_ERROR_EX(category,format,...)	    OB_LOG_BASE(ob::LogType::Error,category, format, __VA_ARGS__)


//@―---------------------------------------------------------------------------
//! @brief      アプリケーションが不正な状態になる可能性がある問題を通知
//!
//! @details    アプリケーションの継続が危険であり、早急に終了することが望まれる。
//! @param format   フォーマット文字列
//! @param args     フォーマット引数
//@―---------------------------------------------------------------------------
#define LOG_ERROR(format,...)                   LOG_ERROR_EX(LOG_DEFAULT_CATEGORY,format,__VA_ARGS__)


//@―---------------------------------------------------------------------------
//! @brief      アプリケーション実行中に発生した軽微な問題を通知
//!
//! @details    アプリケーションの継続が可能である。
//! @param category カテゴリ名
//! @param format   フォーマット文字列
//! @param args     フォーマット引数
//@―---------------------------------------------------------------------------
#define LOG_WARNING_EX(category,format,...)	    OB_LOG_BASE(ob::LogType::Warning,category, format, __VA_ARGS__)
//@―---------------------------------------------------------------------------
//! @brief      アプリケーション実行中に発生した軽微な問題を通知
//!
//! @details    アプリケーションの継続が可能である。
//! @param format   フォーマット文字列
//! @param args     フォーマット引数
//@―---------------------------------------------------------------------------
#define LOG_WARNING(format,...)                 LOG_WARNING_EX(LOG_DEFAULT_CATEGORY,format,__VA_ARGS__)


//@―---------------------------------------------------------------------------
//! @brief      アプリケーション実行中に発生した軽微な問題を通知
//!
//! @details    アプリケーションの継続が可能である。
//! @param category カテゴリ名
//! @param format   フォーマット文字列
//! @param args     フォーマット引数
//@―---------------------------------------------------------------------------
#define LOG_INFO_EX(category,format,...)        OB_LOG_BASE(ob::LogType::Info,category, format, __VA_ARGS__)
//@―---------------------------------------------------------------------------
//! @brief      アプリケーション実行中に発生した軽微な問題を通知
//!
//! @details    アプリケーションの継続が可能である。
//! @param format   フォーマット文字列
//! @param args     フォーマット引数
//@―---------------------------------------------------------------------------
#define LOG_INFO(format,...)                    LOG_INFO_EX(LOG_DEFAULT_CATEGORY,format,__VA_ARGS__)