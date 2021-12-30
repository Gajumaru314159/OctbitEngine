//***********************************************************
//! @file
//! @brief		CriticalSectionテスト
//! @author		Gajumaru
//! 
//! @note       スレッドのテストは失敗しない可能性もあるので失敗のテストはいていません。
//!             コンパイルエラー検知のため成功パターンのみテストしています。
//***********************************************************
#pragma once
/*
#include <Runtime/Foundation/Log/LogMacro.h>

#include <thread>
#include <vector>

using namespace ob;


class Hoge {
public:

    Hoge() {
        m_criticalSection.Startup();
        m_conter = 0;
    }

    ~Hoge() {
        m_criticalSection.Cleanup();
    }

    void HeavyFunc() {
        m_criticalSection.Begin();
        for (s32 i = 0; i < 10000; ++i) {
            ++m_conter;
        }
        m_criticalSection.End();
    }
    void ScopedHeaviFunc() {
        ScopedCriticalSection scs(m_criticalSection);
        for (s32 i = 0; i < 10000; ++i) {
            ++m_conter;
        }
    }

    s32 GetCount()const {
        return m_conter;
    }

private:
    CriticalSection m_criticalSection;
    s32 m_conter;
};


//-----------------------------------------------------------------------------
// Begin / End テスト
//-----------------------------------------------------------------------------
TEST(CriticalSection, Block) {
    Hoge hoge;

    auto func = [&hoge]() {
        hoge.HeavyFunc();
    };

    std::vector<std::thread> threads;
    for (s32 i = 0; i < 8; ++i) {
        threads.emplace_back(func);
    }
    for (auto& th : threads) {
        th.join();
    }

    s32 count = hoge.GetCount();
    EXPECT_EQ(count, 10000 * 8);
}


//-----------------------------------------------------------------------------
// Scopeテスト
//-----------------------------------------------------------------------------
TEST(CriticalSection, ScopedBlock) {
    Hoge hoge;

    auto func = [&hoge]() {
        hoge.ScopedHeaviFunc();
    };

    std::vector<std::thread> threads;
    for (s32 i = 0; i < 8; ++i) {
        threads.emplace_back(func);
    }
    for (auto& th : threads) {
        th.join();
    }

    s32 count = hoge.GetCount();
    EXPECT_EQ(count, 10000 * 8);
}


//-----------------------------------------------------------------------------
// Validテスト
//-----------------------------------------------------------------------------
TEST(CriticalSection, Valid) {
    CriticalSection cs;
    EXPECT_FALSE(cs.IsValid());
    
    cs.Startup();
    EXPECT_TRUE(cs.IsValid());
    cs.Cleanup();
    
    EXPECT_FALSE(cs.IsValid());
}
*/