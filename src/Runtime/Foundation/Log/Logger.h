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
#include <Foundation/Template/String/String.h>
#include "LogType.h"

// Check if fmt/format.h compiles with the X11 index macro defined.
#define index(x, y) no nice things
#include <fmt/format.h>
#undef index

namespace ob
{

    class Logger:public singleton<Logger>
    {
    public:

        enum
        {
            MESSAGE_MAX = 2048, //! メッセージの最大バイト数
        };

        using EventNotifier = event_notifier<const Log&>;        //!< ログ・イベント通知 
        using EventDelegate = EventNotifier::delegate_type;      //!< イベント・デリゲート
        using EventHandle   = EventNotifier::handle;             //!< イベント・ハンドル

    public:

        void Add(LogType type, const Char* category, const SourceLocation& sourceLocation, const Char* pMessage);  // ログの追加

        template<typename... Args>
        void Add(LogType type, const Char* category, const SourceLocation& sourceLocation, const Char* pFormat, Args... args) {
            const String message = fmt::format(pFormat, ob::forward<Args>(args)...);
            Add(type, category, sourceLocation, message.c_str());
        }

        void AddEvent(EventHandle& handle, EventDelegate& delegate);    // ログ・イベントの追加
        void RemoveEvent(EventHandle& handle);                          // ログ・イベントの削除

    private:

        EventNotifier m_eventList;

    };

}// namespace ob