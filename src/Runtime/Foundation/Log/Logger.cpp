//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "Logger.h"
#include "LogType.h"

namespace ob
{

    //@―---------------------------------------------------------------------------
    //! @brief                  ログの追加
    //! 
    //! @details                この関数の呼び出しは LOG_INFO_EX や LOG_WARNING_EX マクロから呼び出される。
    //! @param type             ログの種類
    //! @param category         カテゴリ名
    //! @param fileName         ログ生成場所のファイル名
    //! @param functionName     ログ生成場所の関数名
    //! @param line             ログ生成場所の行数
    //! @param pMessage         メッセージ
	//@―---------------------------------------------------------------------------
    void Logger::Add(LogType type, const SourceLocation& sourceLocation, const Char* category, const Char* pMessage)
    {
        Log log;
        log.type = type;
        log.category = category;
        log.sourceLocation = sourceLocation;
        log.message = pMessage;

        // 登録されたすべてのログリスナに通知
        m_eventList.invoke(log);

        {
            lock_guard lock(m_mutex);
            for (auto& event : m_events) {
                event->OnReceived(log);
            }
        }

        // Fatalログはブレークポイントを発生させる
        if (type == LogType::Fatal) {
            CALL_BREAK_POINT();
        }
    }



    //@―---------------------------------------------------------------------------
    //! @brief ログ・イベントの追加
    //@―---------------------------------------------------------------------------
    void Logger::AddEvent(gsl::not_null<ILogEvent*> logEvent) {
        lock_guard lock(m_mutex);
        m_events.push_back(logEvent.get());
        (*logEvent).OnRegistered();
    }


    //@―---------------------------------------------------------------------------
    //! @brief ログ・イベントの削除
    //@―---------------------------------------------------------------------------
    void Logger::RemoveEvent(ILogEvent* const logEvent) {
        lock_guard lock(m_mutex);
        m_events.remove(logEvent);
        (*logEvent).OnUnregistered();
    }
    

    //@―---------------------------------------------------------------------------
    //! @brief ログ・イベントの追加
    //@―---------------------------------------------------------------------------
    void Logger::AddEvent(EventHandle& handle, EventDelegate& delegate)
    {
        m_eventList.add(handle, delegate);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ログ・イベントの削除
    //@―---------------------------------------------------------------------------
    void Logger::RemoveEvent(EventHandle& handle)
    {
        m_eventList.remove(handle);
    }

}// namespace ob