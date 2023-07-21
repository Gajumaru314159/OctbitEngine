//***********************************************************
//! @file
//! @brief		Path のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Core.h>
#include <Framework/Core/Utility/Global.h>

using namespace ob;

namespace test {

    class TestSystemA {
    public:
        TestSystemA() {

        }
        void func() {
            LOG_INFO("TestSystemA");
        }
    };

    class TestSystemB {
    public:
        TestSystemB(s32 value) {
            m_value = value;
        }
        void func() {
            LOG_INFO("TestSystemB : {}",m_value);
        }
    private:
        s32 m_value;
    };

}

TEST(Global, Global)
{
    if (Global<test::TestSystemA>::Instance() == nullptr) {
        LOG_INFO("TestSystemAはインスタン化されていません");
    }
    {
        Global<test::TestSystemA> test;
        if (auto system = Global<test::TestSystemA>::Instance()) {
            system->func();
        }
    }
    if (Global<test::TestSystemA>::Instance() == nullptr) {
        LOG_INFO("TestSystemAはインスタン化されていません");
    }
    {
        if (auto system = Global<test::TestSystemA>::Instance()) {
            system->func();
        }
    }
    if (Global<test::TestSystemB>::Instance() == nullptr) {
        LOG_INFO("TestSystemBはインスタン化されていません");
    }
}