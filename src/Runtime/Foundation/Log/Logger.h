//***********************************************************
//! @file
//! @brief		ログ出力
//! @author		Gajumaru
//***********************************************************
#pragma once

#include <Foundation/Base/Fwd.h>
#include <Foundation/Template/singleton.h>
#include <Foundation/Template/delegate/delegate.h>
#include <Foundation/Template/event/event_notifier.h>
#include <Foundation/Template/Container/list.h>
#include <Foundation/Template/String/String.h>
#include "LogType.h"
#include "ILogEvent.hpp"

// Check if fmt/format.h compiles with the X11 index macro defined.
#define index(x, y) no nice things
#include <fmt/format.h>
#undef index

namespace ob
{

    //@―---------------------------------------------------------------------------
    //! @brief      ロガー
    //! 
    //! @details    登録したイベントにログの追加を通知する。@n
    //!             このクラスではログの登録と通知のみを行うため、ログウィンドウへの
    //!             出力やログファイルへの保存をする場合は別途実装し登録する必要があ
    //!             る。
    //@―---------------------------------------------------------------------------
    class Logger:public singleton<Logger>
    {
    public:


        //@―---------------------------------------------------------------------------
        //! @brief メッセージの最大バイト数
        //@―---------------------------------------------------------------------------
        enum
        {
            MESSAGE_MAX = 2048, //! メッセージの最大バイト数
        };

        using EventNotifier = event_notifier<const Log&>;        //!< ログ・イベント通知 
        using EventDelegate = EventNotifier::delegate_type;      //!< イベント・デリゲート
        using EventHandle   = EventNotifier::handle;             //!< イベント・ハンドル

    public:

        //@―-------------------------------------------------------------------------------
        //! @brief                  ログの追加
        //! 
        //! @details                この関数の呼び出しは LOG_INFO_EX や LOG_WARNING_EX マクロから呼び出される。@n
        //!                         直接呼び出しは非推奨です。
        //! @param type             ログの種類
        //! @param sourceLocation   ログ生成場所
        //! @param category         カテゴリ名
        //! @param pMessage         メッセージ
        //@―-------------------------------------------------------------------------------
        void Add(LogType type, const SourceLocation& sourceLocation, const Char* category, const Char* pMessage);  // ログの追加


        //@―-------------------------------------------------------------------------------
        //! @brief                  ログの追加
        //! 
        //! @details                この関数の呼び出しは LOG_INFO_EX や LOG_WARNING_EX マクロから呼び出される。@n
        //!                         直接呼び出しは非推奨です。
        //! @param type             ログの種類
        //! @param sourceLocation   ログ生成場所
        //! @param category         カテゴリ名
        //! @param pFormat          フォーマット文字列
        //! @param ...args          フォーマット引数
        //@―-------------------------------------------------------------------------------
        template<typename... Args>
        void Add(LogType type, const SourceLocation& sourceLocation, const Char* category, const Char* pFormat, Args... args) {
            const String message = fmt::format(pFormat, ob::forward<Args>(args)...);
            Add(type, sourceLocation, category, message.c_str());
        }


        //@―-------------------------------------------------------------------------------
        //! @brief ログ・イベントの追加
        //@―-------------------------------------------------------------------------------
        void AddEvent(gsl::not_null<ILogEvent*> logEvent);


        //@―-------------------------------------------------------------------------------
        //! @brief ログ・イベントの削除
        //@―-------------------------------------------------------------------------------
        void RemoveEvent(ILogEvent* const logEvent);


        //@―-------------------------------------------------------------------------------
        //! @brief ログ・イベントの追加
        //@―-------------------------------------------------------------------------------
        void AddEvent(EventHandle& handle, EventDelegate& delegate);


        //@―-------------------------------------------------------------------------------
        //! @brief ログ・イベントの削除
        //@―-------------------------------------------------------------------------------
        void RemoveEvent(EventHandle& handle);

    private:
        mutex m_mutex;
        list<ILogEvent*> m_events;
        EventNotifier m_eventList;

    };

}// namespace ob