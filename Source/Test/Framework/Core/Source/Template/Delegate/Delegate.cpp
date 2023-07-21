//***********************************************************
//! @file
//! @brief		Delegate テスト
//! @author		Gajumaru
//***********************************************************
using namespace ob;
#include <Framework/Core/Template/Utility/Delegate.h>

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


using delegate_type = Delegate<void(int)>;

TEST(Delegate, Construct) {
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
        delegate_type d(lt, &LogTest::LogEvent);
        d(3);
        ASSERT_EQ(s_calledNo, 3);
    }
    // constメソッド
    {
        delegate_type d(lt, &LogTest::LogEventConst);
        d(4);
        ASSERT_EQ(s_calledNo, 4);
    }
    // static関数
    {
        delegate_type d(&LogTest::LogEventStatic);
        d(5);
        ASSERT_EQ(s_calledNo, 5);
    }
    // コピー
    {
        delegate_type d(LogEvent);
        delegate_type d2 = d;
        d = {};
        s_calledNo = 0;
        d2(1);
        ASSERT_EQ(s_calledNo, 1);
    }
    // ムーブ
    {
        delegate_type d(LogEvent);
        delegate_type d2 = std::move(d);
        s_calledNo = 0;
        d2(1);
        ASSERT_EQ(s_calledNo, 1);
    }
}


TEST(Delegate, Op) {
    LogTest lt;

    //　等価
    {
        delegate_type d1(LogEvent);
        delegate_type d1_2(LogEvent);
        delegate_type d2(lamda);
        delegate_type d3(lt, &LogTest::LogEventConst);

        ASSERT_EQ(d1, d1);
        ASSERT_EQ(d1, d1_2);
        ASSERT_NE(d1, d2);
        ASSERT_NE(d1, d3);
    }
    // コピー代入
    {
        delegate_type d(LogEvent);
        delegate_type d2;
        d2 = d;
        d = {};
        s_calledNo = 0;
        d2(1);
        ASSERT_EQ(s_calledNo, 1);
    }
    // ムーブ代入
    {
        delegate_type d(LogEvent);
        delegate_type d2;
        d2 = std::move(d);
        s_calledNo = 0;
        d2(1);
        ASSERT_EQ(s_calledNo, 1);
    }
}

TEST(Delegate, Misc) {
    // 未登録テスト
    {
        s_calledNo = 0;
        delegate_type d;
        d(1);
        ASSERT_EQ(s_calledNo, 0);
    }
    // operator bool()
    {
        delegate_type d;
        ASSERT_TRUE(!d);
        d = LogEvent;
        ASSERT_FALSE(!d);
        d = {};
        ASSERT_TRUE(!d);
    }
}