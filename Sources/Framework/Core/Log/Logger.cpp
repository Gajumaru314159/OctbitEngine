//***********************************************************
//! @file
//! @brief		ログ出力
//! @author		Gajumaru
//***********************************************************
#include "Logger.h"
#include "LogType.h"
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Thread/ScopeLock.h>
#include <MagicEnum/magic_enum.hpp>

namespace ob::core {

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
    void Logger::addLog(LogType type, const SourceLocation& sourceLocation, const Char* category, const Char* pMessage) {
        Log log;
        log.type = type;
        log.category = category;
        log.sourceLocation = sourceLocation;
        log.message = pMessage;

        // 登録されたすべてのログリスナに通知
        {
            ScopeLock<decltype(m_mutex)> lock(m_mutex);
            m_notifier.invoke(log);
        }

        auto logTypeNameAscii = magic_enum::enum_name(type);
        Char logTypeName[10];
        for (s32 i = 0; i < logTypeNameAscii.size(); ++i) {
            logTypeName[i] = static_cast<char>(logTypeNameAscii[i]);
        }
        logTypeName[logTypeNameAscii.size()] = TC('\0');


        auto msg = fmt::format(TC("[{0}][{1}] {2}"),
            logTypeName,
            category,
            pMessage);

        WString ws;
        StringEncoder::Encode(msg, ws);
        ::OutputDebugLog(ws.c_str());
        
        // TODO エラーダイアログ表示は外部から設定する？
        if (type == LogType::Fatal) {
            ::ShowMessageBox(ws.c_str());
            ::CallBreakPoint();
            assert(false);
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

}// namespace ob::core