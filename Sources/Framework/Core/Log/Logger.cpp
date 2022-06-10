//***********************************************************
//! @file
//! @brief		ログ出力
//! @author		Gajumaru
//***********************************************************
#include "Logger.h"
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Thread/ScopeLock.h>
#include <Framework/Core/Utility/Scope.h>
#include <MagicEnum/magic_enum.hpp>

#include <Framework/Core/Log/LogMacro.h>

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
                case LogLevel::Info:    typeName = TC("\033[36m[Info]   \033[m"); break;// シアン
                case LogLevel::Warning: typeName = TC("\033[33m[Warning]\033[m"); break;// 黄色
                case LogLevel::Error:   typeName = TC("\033[31m[Error]  \033[m"); break;// 赤
                case LogLevel::Fatal:   typeName = TC("\033[35m[Fatal]  \033[m"); break;// ピンク
                default:                typeName = TC("\033[37m[Unknown]\033[m"); break;
                }
                // フォーマット
                auto msg = fmt::format(TC("{} {}"), typeName, log.message);
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
                case LogLevel::Info:    typeName = TC("[Info]   "); break;// シアン
                case LogLevel::Warning: typeName = TC("[Warning]"); break;// 黄色
                case LogLevel::Error:   typeName = TC("[Error]  "); break;// 赤
                case LogLevel::Fatal:   typeName = TC("[Fatal]  "); break;// ピンク
                default:                typeName = TC("[Unknown]"); break;
                }
                // フォーマット
                auto msg = fmt::format(TC("{} {}"), typeName, log.message);
                WString ws;
                StringEncoder::Encode(msg, ws);

                // 出力
                ::OutputDebugLog(ws.c_str());


                auto msg2 = fmt::format(TC("{}({})\n"), log.sourceLocation.filePath, log.sourceLocation.line);
                StringEncoder::Encode(msg2, ws);
                ::OutputDebugLog(ws.c_str());

                // エラーダイアログ表示
                if (log.level == LogLevel::Fatal) {
                    ::ShowMessageBox(ws.c_str());
                    ::CallBreakPoint();
                }
            }
#endif

        };

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

        // デバッグログに出力
        //{
        //    auto logTypeNameAscii = magic_enum::enum_name(type);
        //    Char logTypeName[10];
        //    for (s32 i = 0; i < logTypeNameAscii.size(); ++i) {
        //        logTypeName[i] = static_cast<char>(logTypeNameAscii[i]);
        //    }
        //    logTypeName[logTypeNameAscii.size()] = TC('\0');


        //    auto msg = fmt::format(TC("[{0}][{1}] {2}"),
        //        logTypeName,
        //        category,
        //        pMessage);

        //    WString ws;
        //    StringEncoder::Encode(msg, ws);
        //    ::OutputDebugLog(ws.c_str());

        //    // TODO エラーダイアログ表示は外部から設定する？
        //    if (type == LogType::Fatal) {
        //        ::ShowMessageBox(ws.c_str());
        //        ::CallBreakPoint();
        //        assert(false);
        //    }
        //}

        if (log.level == LogLevel::Fatal) {
            ::CallBreakPoint();
            assert(false);
        }

        m_logged = false;
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