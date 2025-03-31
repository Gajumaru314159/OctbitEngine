//***********************************************************
//! @file
//! @brief		Loggerテスト
//! @author		Gajumaru
//***********************************************************
namespace ob {

    namespace {

        static LogLevel s_logType;
        static String s_message;

        void LogEvent(const Log& log)
        {
            s_logType = log.level;

            s32 index = 0;

            s_message = log.message;
        }

        class LogTest
        {
        public:
            LogTest() = default;

            void LogEvent(LogLevel level, const Char* pMessage)
            {
                s_logType = level;
                s_message = pMessage;
            }

            void LogEventConst(LogLevel level, const Char* pMessage)const
            {
                s_logType = level;
                s_message = pMessage;
            }

            static void LogEventStatic(LogLevel level, const Char* pMessage)
            {
                s_logType = level;
                s_message = pMessage;
            }
        };
    }

    TEST(Logger, Subscribe) {

        Logger logger;

        // ログイベントを登録
        Logger::EventHandle handle;
        if(auto logger = Logger::Get()) logger->addEvent(handle, { LogEvent });

        // ログ追加
        {
            String message = "Traceログチェック";
            LOG_TRACE("[{0}]", message);

            ASSERT_EQ(s_logType, LogLevel::Trace);
            ASSERT_EQ(s_message, "[Traceログチェック]");
        }
        {
            String message = "Infoログチェック";
            LOG_INFO("[{0}]", message);

            ASSERT_EQ(s_logType, LogLevel::Info);
            ASSERT_EQ(s_message, "[Infoログチェック]");
        }
        {
            String message = "Warningログチェック";
            LOG_WARNING("[{0}]", message);

            ASSERT_EQ(s_logType, LogLevel::Warning);
            ASSERT_EQ(s_message, "[Warningログチェック]");
        }
        {
            String message = "Errorログチェック";
            LOG_ERROR("[{0}]", message);

            ASSERT_EQ(s_logType, LogLevel::Error);
            ASSERT_EQ(s_message, "[Errorログチェック]");
        }
        if(false){
            String message = "Fatalログチェック";
            LOG_FATAL("[{0}]", message);

            ASSERT_EQ(s_logType, LogLevel::Fatal);
            ASSERT_EQ(s_message, "[Fatalログチェック]");
        }

        // ログイベントの削除
        if (auto logger = Logger::Get()) logger->removeEvent(handle);

        // ログ追加
        {
            String message = "購読解除";
            LOG_INFO("[{0}]", message);

            ASSERT_NE(s_logType , LogLevel::Info);
            ASSERT_NE(s_message, "[購読解除]");
        }

    }

}