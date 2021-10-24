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
    //! @details                この関数の呼び出しは LOG_INFO_EX や LOG_WARNING_EX マクロから呼び出される。@n
    //!                         直接呼び出しは非推奨です。
    //! @param type             ログの種類
    //! @param sourceLocation   ログ生成場所
    //! @param category         カテゴリ名
    //! @param pMessage         メッセージ
    //@―---------------------------------------------------------------------------
    void Logger::AddLog(ELogType type, const SourceLocation& sourceLocation, const Char* category, const Char* pMessage)
    {
        Log log;
        log.type = type;
        log.category = category;
        log.sourceLocation = sourceLocation;
        log.message = pMessage;

        // 登録されたすべてのログリスナに通知
        {
            lock_guard lock(m_mutex);
            for (auto& event : m_events) {
                event->OnReceived(log);
            }
        }

        // Fatalログはブレークポイントを発生させる
        if (type == ELogType::Fatal) {
            CALL_BREAK_POINT();
        }
    }



    //@―---------------------------------------------------------------------------
    //! @brief ログ・イベントの追加
    //@―---------------------------------------------------------------------------
    void Logger::AddListener(ILogEvent* pLogEvent) {
        if (!pLogEvent)return;
        lock_guard lock(m_mutex);
        m_events.push_back(pLogEvent);
        pLogEvent->OnRegistered();
    }


    //@―---------------------------------------------------------------------------
    //! @brief ログ・イベントの削除
    //@―---------------------------------------------------------------------------
    void Logger::RemoveListener(ILogEvent* pLogEvent) {
        if (!pLogEvent)return;
        lock_guard lock(m_mutex);
        m_events.remove(pLogEvent);
        pLogEvent->OnUnregistered();
    }
    
}// namespace ob