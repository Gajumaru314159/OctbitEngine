//***********************************************************
//! @file
//! @brief		event_notifierテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Template/Event/EventNotifier.h>
#include <vector>
using namespace ob;

static int s_num;
static std::vector<int> s_primes{ 2,3,5,7,11,13,17,19 };

void Func(size_t num) {
    s_num *= s_primes[num];
}

class Op {
public:
    void Method(size_t num) {
        s_num *= s_primes[num + 1];
    }
    void ConstMethod(size_t num)const {
        s_num *= s_primes[num + 2];
    }
    static void StaticMethod(size_t num) {
        s_num *= s_primes[num + 3];
    }
};

TEST(EventNotifier, Test) {

    using Notifier = EventNotifier<size_t>;
    using Handle = Notifier::Handle;
    Notifier en;

    Op op;

    Handle funcHandle;
    Handle methodHandle;
    Handle constMethodHandle;
    Handle staticMethodHandle;
    {
        Handle lamdaHandle;

        en.add(funcHandle, Func);
        en.add(methodHandle, op, &Op::Method);
        en.add(constMethodHandle, op, &Op::ConstMethod);
        en.add(staticMethodHandle, &Op::StaticMethod);

        auto func = [](size_t num) {
            s_num *= s_primes[num + 4];
        };
        en.add(lamdaHandle, func);

        // invokeテスト
        s_num = 1;
        en.invoke(2);
        CHECK_EQUAL(s_num, 5 * 7 * 11 * 13 * 17);
    }
    // Handleスコープテスト
    s_num = 1;
    en.invoke(1);
    CHECK_EQUAL(s_num, 3 * 5 * 7 * 11);


    en.remove(funcHandle);
    // removeテスト
    s_num = 1;
    en.invoke(0);
    CHECK_EQUAL(s_num, 3 * 5 * 7);

    constMethodHandle.remove();
    // ハンドルremoveテスト
    s_num = 1;
    en.invoke(0);
    CHECK_EQUAL(s_num, 3 * 7);
}