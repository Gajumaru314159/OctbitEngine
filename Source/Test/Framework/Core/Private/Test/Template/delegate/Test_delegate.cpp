﻿//***********************************************************
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


using delegate_type = Delegate<void(int)>;

TEST_GROUP(Delegate) {};

TEST(Delegate, Construct) {
    LogTest lt;

    // 関数
    {
        delegate_type d(LogEvent);
        d(1);
        CHECK_EQUAL(s_calledNo, 1);
    }
    // ラムダ式
    {
        delegate_type d(lamda);
        d(2);
        CHECK_EQUAL(s_calledNo, 2);
    }
    // メソッド
    {
        delegate_type d(lt, &LogTest::LogEvent);
        d(3);
        CHECK_EQUAL(s_calledNo, 3);
    }
    // constメソッド
    {
        delegate_type d(lt, &LogTest::LogEventConst);
        d(4);
        CHECK_EQUAL(s_calledNo, 4);
    }
    // static関数
    {
        delegate_type d(&LogTest::LogEventStatic);
        d(5);
        CHECK_EQUAL(s_calledNo, 5);
    }
    // コピー
    {
        delegate_type d(LogEvent);
        delegate_type d2 = d;
        d.clear();
        s_calledNo = 0;
        d2(1);
        CHECK_EQUAL(s_calledNo, 1);
    }
    // ムーブ
    {
        delegate_type d(LogEvent);
        delegate_type d2 = std::move(d);
        s_calledNo = 0;
        d2(1);
        CHECK_EQUAL(s_calledNo, 1);
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

        CHECK_TRUE(d1 == d1);
        CHECK_TRUE(d1 == d1_2);
        CHECK_FALSE(d1 == d2);
        CHECK_FALSE(d1 == d3);
    }
    // コピー代入
    {
        delegate_type d(LogEvent);
        delegate_type d2;
        d2 = d;
        d.clear();
        s_calledNo = 0;
        d2(1);
        CHECK_EQUAL(s_calledNo, 1);
    }
    // ムーブ代入
    {
        delegate_type d(LogEvent);
        delegate_type d2;
        d2 = std::move(d);
        s_calledNo = 0;
        d2(1);
        CHECK_EQUAL(s_calledNo, 1);
    }
}

TEST(Delegate, Assign) {
    LogTest lt;

    // 関数
    {
        delegate_type d;
        d.assign(LogEvent);
        d(1);
        CHECK_EQUAL(s_calledNo, 1);
    }
    // ラムダ式
    {
        delegate_type d;
        d.assign(lamda);
        d(2);
        CHECK_EQUAL(s_calledNo, 2);
    }
    // メソッド
    {
        delegate_type d;
        d.assign(lt, &LogTest::LogEvent);
        d(3);
        CHECK_EQUAL(s_calledNo, 3);
    }
    // constメソッド
    {
        delegate_type d;
        d.assign(lt, &LogTest::LogEventConst);
        d(4);
        CHECK_EQUAL(s_calledNo, 4);
    }
    // static関数
    {
        delegate_type d;
        d.assign(&LogTest::LogEventStatic);
        d(5);
        CHECK_EQUAL(s_calledNo, 5);
    }
}

TEST(Delegate, Misc) {
    // 未登録テスト
    {
        s_calledNo = 0;
        delegate_type d;
        d(1);
        CHECK_EQUAL(s_calledNo, 0);
    }
    // empty()
    {
        delegate_type d;
        CHECK_TRUE(d.empty());
        d.assign(LogEvent);
        CHECK_FALSE(d.empty());
        d.clear();
        CHECK_TRUE(d.empty());
    }
}