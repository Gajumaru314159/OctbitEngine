﻿//***********************************************************
//! @file
//! @brief		ログ型
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief このマクロの呼び出し位置の SourceLocation オブジェクトを作成する
    //@―---------------------------------------------------------------------------
#ifdef OB_DEBUG
#define CURRENT_SOURCE_LOCATION ob::core::SourceLocation{TC(__FILE__),TC( FUNC_NAME ),__LINE__}
#else
#define CURRENT_SOURCE_LOCATION ob::core::SourceLocation{TC(""),TC(""),__LINE__}
#endif

    //@―---------------------------------------------------------------------------
    //! @brief      ソースコード上の位置を表す構造体
    //! 
    //! @details    構築には CURRENT_SOURCE_LOCATION マクロを使用してください。
    //@―---------------------------------------------------------------------------
    struct SourceLocation {
        const Char* filePath = nullptr;             //!< ファイルパス
        const Char* functionName = nullptr;         //!< 関数名
        s32 line = 0;                               //!< 行番号
    };


    //@―---------------------------------------------------------------------------
    //! @brief ログのレベル
    //@―---------------------------------------------------------------------------
    enum class LogLevel {
        Fatal,      //!< 致命的エラー
        Error,		//!< エラー
        Warning,	//!< 警告表示
        Info,		//!< 情報表示
        Trace,		//!< デバッグ用トレース
    };


    //@―---------------------------------------------------------------------------
    //! @brief ログ・オブジェクト
    //@―---------------------------------------------------------------------------
    struct Log {
        LogLevel        level;              //!< タイプ
        const Char*     category = nullptr; //!< カテゴリ
        const Char*     message = nullptr;  //!< メッセージ
        SourceLocation  sourceLocation;     //!< ログが発生したソースコード上の位置
    };

}