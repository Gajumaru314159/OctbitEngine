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
    void Logger::addLog(LogType type, const SourceLocation& sourceLocation, const Char* category, const Char* pMessage)
    {
        Log log;
        log.type = type;
        log.category = category;
        log.sourceLocation = sourceLocation;
        log.message = pMessage;

        // 登録されたすべてのログリスナに通知
        {
            lock_guard lock(m_mutex);
            m_notifier.invoke(log);
        }

        // Fatalログはブレークポイントを発生させる
        if (type == LogType::Fatal) {
            ::CallBreakPoint();
        }
    }



    //@―---------------------------------------------------------------------------
    //! @brief ログ・イベントの追加
    //@―---------------------------------------------------------------------------
    void Logger::addEvent(EventHandle& handle, EventDelegateType delegate) {
        m_notifier.add(handle, delegate);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ログ・イベントの削除
    //@―---------------------------------------------------------------------------
    void Logger::removeEvent(EventHandle& handle) {
        m_notifier.remove(handle);
    }
    
}// namespace ob