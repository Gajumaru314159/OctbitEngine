//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Log/Logger.h>
#include <Framework/Core/Log/StackTrace.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Thread/ScopeLock.h>
#include <Framework/Core/Utility/Scope.h>

#include <iostream>

namespace ob::core {

    Logger* Logger::s_instance = nullptr;

    //! @brief コンストラクタ
    Logger::Logger() {

        assert(s_instance == nullptr);
        s_instance = this;

        // デフォルトログイベント
        auto func = [](const Log& log) {

            // 標準出力
            if(log.level!=LogLevel::Trace){
                StringView typeName;
                switch (log.level) {
                case LogLevel::Fatal:   typeName = "\033[35m[Fatal]  \033[0m"; break;// マゼンタ
                case LogLevel::Error:   typeName = "\033[31m[Error]  \033[0m"; break;// 赤
                case LogLevel::Warning: typeName = "\033[33m[Warning]\033[0m"; break;// 黄色
                case LogLevel::Info:    typeName = "\033[36m[Info]   \033[0m"; break;// シアン
                case LogLevel::Trace:   typeName = "\033[38;2;128;128;128m[Trace]  "; break;// グレー
                default:                typeName = "\033[32m[Unknown]\033[0m"; break;// 緑
                }
                // フォーマット
                auto msg = Format ("{} {}", typeName, log.message);
                WString ws;
                StringEncoder::Encode(msg, ws);

                // 標準出力
                std::wcout << ws.data() << std::endl;
            }

#ifdef OS_WINDOWS
            // Visual Studio
            if (log.level != LogLevel::Trace || true) {
                StringView typeName;
                switch (log.level) {
                case LogLevel::Fatal:   typeName = "[Fatal]  "; break;
                case LogLevel::Error:   typeName = "[Error]  "; break;
                case LogLevel::Warning: typeName = "[Warning]"; break;
                case LogLevel::Info:    typeName = "[Info]   "; break;
                case LogLevel::Trace:   typeName = "[Trace]  "; break;
                default:                typeName = "[Unknown]"; break;
                }
                // フォーマット
                auto msg = Format("{} {}\n{}({})\n", typeName, log.message,log.sourceLocation.filePath,log.sourceLocation.line);
                WString ws;
                StringEncoder::Encode(msg, ws);

                // 出力
                ::OutputDebugLog(ws.c_str());

                bool outputLine = false;
                if (outputLine) {
                    auto msg2 = Format("{}({})\n", log.sourceLocation.filePath, log.sourceLocation.line);
                    StringEncoder::Encode(msg2, ws);
                    ::OutputDebugLog(ws.c_str());
                }


                // エラーダイアログ表示
                if (log.level == LogLevel::Fatal) {
                    ::ShowMessageBox(ws.c_str());
                }
            }

            if (log.level == LogLevel::Fatal) {

                ::OutputDebugLog(L"********************");
                ::OutputDebugLog(L"* スタックトレース *");
                ::OutputDebugLog(L"********************");
                
                for (auto& s : StackTrace::Capture().elements()) {
                    // auto msg2 = Format("{}\n{}({})\n", s.name, s.filename, s.line);
                    auto msg2 = Format("{}({})",s.filename, s.line);
                    WString ws;
                    StringEncoder::Encode(msg2, ws);
                    ::OutputDebugLog(ws.c_str());
                }
            }
#endif

        };

        // std::wcoutで日本語が表示できないため対応
        setlocale(LC_ALL, "Japanese");


        addEvent(m_hDebugEvent, func);
    }


    //! @brief デストラクタ
    Logger::~Logger() {
        m_hDebugEvent.remove();
        s_instance = nullptr;
    }


    //! @brief                  ログの追加
    //! 
    //! @details                この関数の呼び出しは LOG_INFO_EX や LOG_WARNING_EX マクロから呼び出される。@n
    //!                         直接呼び出しは非推奨です。
    //! @param type             ログの種類
    //! @param sourceLocation   ログ生成場所
    //! @param category         カテゴリ名
    //! @param pMessage         メッセージ
    void Logger::addLog(LogLevel level, const SourceLocation& sourceLocation, const Char* category, const Char* pMessage) {

        thread_local bool logged = false;
        if (logged)return;// 無限ループ回避のため早期リターン

        ScopeLock lock(m_mutex);
        logged = true;

        Log log;
        log.level = level;
        log.category = category;
        log.sourceLocation = sourceLocation;
        log.message = pMessage;

        // 登録されたすべてのリスナに通知
        m_notifier.invoke(log);

        logged = false;
    }


    //! @brief ログ・イベントの追加
    void Logger::addEvent(EventHandle& handle, EventDelegateType func) {
        m_notifier.add(handle, func);
    }


    //! @brief ログ・イベントの削除
    void Logger::removeEvent(EventHandle& handle) {
        m_notifier.remove(handle);
    }

}