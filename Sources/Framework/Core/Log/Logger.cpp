//***********************************************************
//! @file
//! @brief		ログ出力
//! @author		Gajumaru
//***********************************************************
#include "Logger.h"
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Thread/ScopeLock.h>
#include <Framework/Core/Utility/Scope.h>

#include <iostream>

static ob::core::Logger::EventHandle s_debugLogEvent;

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //@―---------------------------------------------------------------------------
    Logger::Logger() {

        // デフォルトログイベント
        auto func = [](const Log& log) {

            // 標準出力
            {
                StringView typeName;
                switch (log.level) {
                case LogLevel::Fatal:   typeName = TC("\033[35m[Fatal]  \033[0m"); break;// マゼンタ
                case LogLevel::Error:   typeName = TC("\033[31m[Error]  \033[0m"); break;// 赤
                case LogLevel::Warning: typeName = TC("\033[33m[Warning]\033[0m"); break;// 黄色
                case LogLevel::Info:    typeName = TC("\033[36m[Info]   \033[0m"); break;// シアン
                case LogLevel::Trace:   typeName = TC("\033[38;2;128;128;128m[Trace]  "); break;// グレー
                default:                typeName = TC("\033[32m[Unknown]\033[0m"); break;// 緑
                }
                // フォーマット
                auto msg = Format (TC("{} {}"), typeName, log.message);
                WString ws;
                StringEncoder::Encode(msg, ws);

                // 標準出力
                std::wcout << ws << std::endl;
            }

#ifdef OS_WINDOWS
            // Visual Studio
            {
                StringView typeName;
                switch (log.level) {
                case LogLevel::Fatal:   typeName = TC("[Fatal]  "); break;
                case LogLevel::Error:   typeName = TC("[Error]  "); break;
                case LogLevel::Warning: typeName = TC("[Warning]"); break;
                case LogLevel::Info:    typeName = TC("[Info]   "); break;
                case LogLevel::Trace:   typeName = TC("[Trace]  "); break;
                default:                typeName = TC("[Unknown]"); break;
                }
                // フォーマット
                auto msg = Format(TC("{} {}"), typeName, log.message);
                WString ws;
                StringEncoder::Encode(msg, ws);

                // 出力
                ::OutputDebugLog(ws.c_str());

                bool outputLine = false;
                if (outputLine) {
                    auto msg2 = Format(TC("{}({})\n"), log.sourceLocation.filePath, log.sourceLocation.line);
                    StringEncoder::Encode(msg2, ws);
                    ::OutputDebugLog(ws.c_str());
                }


                // エラーダイアログ表示
                if (log.level == LogLevel::Fatal) {
                    ::ShowMessageBox(ws.c_str());
                    ::CallBreakPoint();
                }
            }
#endif

        };

        // std::wcoutで日本語が表示できないため対応
        setlocale(LC_ALL, "Japanese");


        addEvent(s_debugLogEvent, func);
    }


    //@―---------------------------------------------------------------------------
    //! @brief デストラクタ
    //@―---------------------------------------------------------------------------
    Logger::~Logger() {
        s_debugLogEvent.remove();
    }


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
    void Logger::addLog(LogLevel level, const SourceLocation& sourceLocation, const Char* category, const Char* pMessage) {

        if (m_logged.load())return;// 無限ループ回避のため早期リターン

        ScopeLock lock(m_mutex);
        m_logged = true;

        Log log;
        log.level = level;
        log.category = category;
        log.sourceLocation = sourceLocation;
        log.message = pMessage;

        // 登録されたすべてのリスナに通知
        m_notifier.invoke(log);

        if (log.level == LogLevel::Fatal) {
            ::CallBreakPoint();
            assert(false);
        }

        m_logged = false;
    }


    //@―---------------------------------------------------------------------------
    //! @brief ログ・イベントの追加
    //@―---------------------------------------------------------------------------
    void Logger::addEvent(EventHandle& handle, EventDelegateType func) {
        m_notifier.add(handle, func);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ログ・イベントの削除
    //@―---------------------------------------------------------------------------
    void Logger::removeEvent(EventHandle& handle) {
        m_notifier.remove(handle);
    }

}// namespace ob::core