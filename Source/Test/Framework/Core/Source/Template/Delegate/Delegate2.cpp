//***********************************************************
//! @file
//! @brief		Delegate テスト
//! @author		Gajumaru
//***********************************************************
using namespace ob;
#include <Framework/Core/Template/Delegate/Delegate2.h>

static int s_calledNo = 0;

static void LogEvent(int arg) {
    s_calledNo = arg;
}

namespace {

    class LogTest {
    public:
        LogTest() = default;

        void LogEvent(int arg) {
            s_calledNo = arg;
        }

        void LogEventConst(int arg)const {
            s_calledNo = arg;
        }

        static void LogEventStatic(int arg) {
            s_calledNo = arg;
        }
    };

}

static auto lamda = [](int arg) {s_calledNo = arg; };


using delegate_type = Delegate2<void(int)>;

TEST(Delegate2, Construct) {
    LogTest lt;

    // 関数
    {
        delegate_type d(LogEvent);
        d(1);
        ASSERT_EQ(s_calledNo, 1);
    }
    // ラムダ式
    {
        delegate_type d(lamda);
        d(2);
        ASSERT_EQ(s_calledNo, 2);
    }
    // メソッド
    {
        delegate_type d(&lt, LogTest::LogEvent);
        d(3);
        ASSERT_EQ(s_calledNo, 3);
    }
    // constメソッド
    {
        delegate_type d(&lt, LogTest::LogEventConst);
        d(4);
        ASSERT_EQ(s_calledNo, 4);
    }
    // static関数
    {
        delegate_type d(LogTest::LogEventStatic);
        d(5);
        ASSERT_EQ(s_calledNo, 5);
    }
}


TEST(Delegate, Op) {
    LogTest lt;

    //　等価
    {
        delegate_type d1(LogEvent);
        delegate_type d2(lamda);
        delegate_type d3(&lt, LogTest::LogEventConst);

        ASSERT_TRUE(d1);
        ASSERT_TRUE(d2);
        ASSERT_TRUE(d3);

        d1 = {};
        d2 = {};
        d3 = {};

        ASSERT_TRUE(d1);
        ASSERT_TRUE(d2);
        ASSERT_TRUE(d3);
    }
}