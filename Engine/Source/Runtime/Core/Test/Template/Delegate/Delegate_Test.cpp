//***********************************************************
//! @file
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
    if(0){
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


namespace {

    int s_counter = 0;

    class DestructTester {
    public:
        DestructTester() {
            s_counter++;
        }
        ~DestructTester() {
            s_counter--;
        }
        DestructTester(const DestructTester&) {
            s_counter++;
        }
        DestructTester& operator=(const DestructTester&) {
            s_counter++;
            return *this;
        }
    };
}

// デリゲートにアサインした関数オブジェクトのデストラクタが呼び出されるか
TEST(Delegate, Destructor) {
    ASSERT_EQ(s_counter, 0);
    {
        DestructTester obj;
        delegate_type d = [obj](int) {};
        ASSERT_EQ(s_counter,2);
    }
    ASSERT_EQ(s_counter, 0);
    {
        DestructTester obj;
        delegate_type d = [obj](int) {};
        delegate_type d2 = d;
        ASSERT_EQ(s_counter, 3);
    }
    ASSERT_EQ(s_counter, 0);
    {
        DestructTester obj;
        delegate_type d = [obj](int) {};
        delegate_type d2 = std::move(d);
        ASSERT_EQ(s_counter, 2);
    }
    ASSERT_EQ(s_counter, 0);


    {
        DestructTester obj;
        delegate_type d = [&obj](int) {};
        ASSERT_EQ(s_counter, 1);
    }
    ASSERT_EQ(s_counter, 0);
    {
        DestructTester obj;
        delegate_type d = [&obj](int) {};
        delegate_type d2 = d;
        ASSERT_EQ(s_counter, 1);
    }
    ASSERT_EQ(s_counter, 0);
    {
        DestructTester obj;
        delegate_type d = [&obj](int) {};
        delegate_type d2 = std::move(d);
        ASSERT_EQ(s_counter, 1);
    }
    ASSERT_EQ(s_counter, 0);

}