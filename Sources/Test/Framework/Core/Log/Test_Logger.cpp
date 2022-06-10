//***********************************************************
//! @file
//! @brief		Loggerテスト
//! @author		Gajumaru
//***********************************************************
using namespace ob;


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


TEST(Logger, AddLogEvent) {
    Logger::EventDelegateType event = LogEvent;

    // ログイベントを登録
    Logger::EventHandle handle;
    Logger::Instance().addEvent(handle, event);

    // ログ追加
    const Char message1[] = TC("Hoge");
    LOG_INFO("{0}",message1);

    EXPECT_EQ(s_logType, LogLevel::Info);
    EXPECT_TRUE(s_message, message1);

    // ログ追加
    LOG_WARNING("Hoge{0}{1}",2,TC("A"));

    EXPECT_EQ(s_logType, LogLevel::Warning);
    EXPECT_TRUE(s_message, TC("Hoge2A"));

    // ログイベントの削除
    Logger::Instance().removeEvent(handle);

    // ログ追加
    const Char message2[] = TC("Fuga");
    LOG_ERROR("{0}",message2);

    EXPECT_FALSE(s_logType == LogLevel::Error);
    EXPECT_TRUE(s_message, message2);

}