//***********************************************************
//! @file
//! @brief		delegateテスト
//! @author		Gajumaru
//***********************************************************
using namespace ob;
#include <Framework/Core/Template/Delegate/Delegate.h>

static int s_calledNo = 0;

void LogEvent(int arg) {
    s_calledNo = arg;
}

class LogTest {
public:
    LogTest() = default;

    void LogEvent(int arg)     {
        s_calledNo = arg;
    }

    void LogEventConst(int arg)const     {
        s_calledNo = arg;
    }

    static void LogEventStatic(int arg)     {
        s_calledNo = arg;
    }
};

auto lamda = [](int arg) {s_calledNo = arg; };


using delegate_type = delegate<void(int)>;

TEST(delegate, Construct) {
    LogTest lt;

    // 関数
    {
        delegate_type d(LogEvent);
        d(1);
        EXPECT_EQ(s_calledNo, 1);
    }
    // ラムダ式
    {
        delegate_type d(lamda);
        d(2);
        EXPECT_EQ(s_calledNo, 2);
    }
    // メソッド
    {
        delegate_type d(lt, &LogTest::LogEvent);
        d(3);
        EXPECT_EQ(s_calledNo, 3);
    }
    // constメソッド
    {
        delegate_type d(lt, &LogTest::LogEventConst);
        d(4);
        EXPECT_EQ(s_calledNo, 4);
    }
    // static関数
    {
        delegate_type d(&LogTest::LogEventStatic);
        d(5);
        EXPECT_EQ(s_calledNo, 5);
    }
    // コピー
    {
        delegate_type d(LogEvent);
        delegate_type d2 = d;
        d.clear();
        s_calledNo = 0;
        d2(1);
        EXPECT_EQ(s_calledNo, 1);
    }
    // ムーブ
    {
        delegate_type d(LogEvent);
        delegate_type d2 = std::move(d);
        s_calledNo = 0;
        d2(1);
        EXPECT_EQ(s_calledNo, 1);
    }
}


TEST(delegate, Op) {
    LogTest lt;

    //　等価
    {
        delegate_type d1(LogEvent);
        delegate_type d1_2(LogEvent);
        delegate_type d2(lamda);
        delegate_type d3(lt, &LogTest::LogEventConst);

        EXPECT_EQ(d1, d1);
        EXPECT_EQ(d1, d1_2);
        EXPECT_NE(d1, d2);
        EXPECT_NE(d1, d3);
    }
    // コピー代入
    {
        delegate_type d(LogEvent);
        delegate_type d2;
        d2 = d;
        d.clear();
        s_calledNo = 0;
        d2(1);
        EXPECT_EQ(s_calledNo, 1);
    }
    // ムーブ代入
    {
        delegate_type d(LogEvent);
        delegate_type d2;
        d2 = std::move(d);
        s_calledNo = 0;
        d2(1);
        EXPECT_EQ(s_calledNo, 1);
    }
}

TEST(delegate, Assign) {
    LogTest lt;

    // 関数
    {
        delegate_type d;
        d.assign(LogEvent);
        d(1);
        EXPECT_EQ(s_calledNo, 1);
    }
    // ラムダ式
    {
        delegate_type d;
        d.assign(lamda);
        d(2);
        EXPECT_EQ(s_calledNo, 2);
    }
    // メソッド
    {
        delegate_type d;
        d.assign(lt, &LogTest::LogEvent);
        d(3);
        EXPECT_EQ(s_calledNo, 3);
    }
    // constメソッド
    {
        delegate_type d;
        d.assign(lt, &LogTest::LogEventConst);
        d(4);
        EXPECT_EQ(s_calledNo, 4);
    }
    // static関数
    {
        delegate_type d;
        d.assign(&LogTest::LogEventStatic);
        d(5);
        EXPECT_EQ(s_calledNo, 5);
    }
}

TEST(delegate, Misc) {
    // 未登録テスト
    {
        s_calledNo = 0;
        delegate_type d;
        d(1);
        EXPECT_EQ(s_calledNo, 0);
    }
    // empty()
    {
        delegate_type d;
        EXPECT_TRUE(d.empty());
        d.assign(LogEvent);
        EXPECT_FALSE(d.empty());
        d.clear();
        EXPECT_TRUE(d.empty());
    }
}