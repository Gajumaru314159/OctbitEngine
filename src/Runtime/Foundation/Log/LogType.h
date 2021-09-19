﻿//***********************************************************
//! @file
//! @brief		ログ型
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include <Foundation/Template/delegate/delegate.h>

namespace ob
{
    //@―---------------------------------------------------------------------------
    //! @brief このマクロの呼び出し位置の SourceLocation オブジェクトを作成する
    //@―---------------------------------------------------------------------------
    #define CURRENT_SOURCE_LOCATION SourceLocation{TEXT(__FILE__),TEXT( __FUNC_NAME__ ),__LINE__}

    //@―---------------------------------------------------------------------------
    //! @brief      ソースコード上の位置を表す構造体
    //! 
    //! @details    構築には CURRENT_SOURCE_LOCATION マクロを使用してください。
    //@―---------------------------------------------------------------------------
    struct SourceLocation {
        const Char* filePath = nullptr;
        const Char* functionName = nullptr;
        s32 line = 0;
    };


    //@―---------------------------------------------------------------------------
    //! @brief ログの種類
    //@―---------------------------------------------------------------------------
    enum class LogType
    {
        Fatal,      //!< 致命的エラー
        Error,		//!< エラー
        Warning,	//!< 警告表示
        Info,		//!< 情報表示
    };


    //@―---------------------------------------------------------------------------
    //! @brief ログ・オブジェクト
    //@―---------------------------------------------------------------------------
    struct Log
    {
        LogType         type;           //!< タイプ
                                        //   時間
        const Char*     category;       //!< カテゴリ
        const Char*     message;        //!< メッセージ
        SourceLocation  sourceLocation; //!< ログが発生したソースコード上の位置
                                        //   スレッドID
    };

}// namespcae ob