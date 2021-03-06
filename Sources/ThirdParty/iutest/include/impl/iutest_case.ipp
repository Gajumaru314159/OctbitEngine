//======================================================================
//-----------------------------------------------------------------------
/**
 * @file        iutest_case.ipp
 * @brief       testcase implimentation
 *
 * @author      t.shirayanagi
 * @par         copyright
 * Copyright (C) 2011-2019, Takazumi Shirayanagi\n
 * This software is released under the new BSD License,
 * see LICENSE
*/
//-----------------------------------------------------------------------
//======================================================================
#ifndef INCG_IRIS_IUTEST_CASE_IPP_F57C9B7E_7CAA_4429_BE75_FCAAEED1B220_
#define INCG_IRIS_IUTEST_CASE_IPP_F57C9B7E_7CAA_4429_BE75_FCAAEED1B220_

//======================================================================
// include
#include "../iutest_case.hpp"

namespace iutest
{

IUTEST_IPP_INLINE bool TestCase::Run()
{
    if( !should_run() )
    {
        return true;
    }

    if( TestFlag::IsEnableFlag(TestFlag::SHUFFLE_TESTS) )
    {
        detail::RandomShuffle(m_testinfos, TestEnv::genrand());
    }

    // テスト開始
    bool result=false;

    m_start_timestamp = detail::GetTimeInMillis();
    TestEnv::event_listeners().OnTestCaseStart(*this);
#if IUTEST_HAS_EXCEPTIONS
    if( TestFlag::IsEnableFlag(TestFlag::CATCH_EXCEPTION_EACH) )
    {
        try
        {
            result = RunImpl();
        }
        catch( const TestPartResult::Type& eType )
        {
            CheckSetUpSkipped();

            if( TestPartResult::type_is_failed(eType) && TestFlag::IsEnableFlag(TestFlag::THROW_ON_FAILURE) )
            {
                throw;
            }
        }
        catch( ... )
        {
            throw;
        }
    }
    else
#endif
    {
        result = RunImpl();
    }
    // テスト終了
    TestEnv::event_listeners().OnTestCaseEnd(*this);

    return result;
}

IUTEST_IPP_INLINE bool TestCase::RunImpl()
{
    bool result=true;
    m_elapsedmsec = 0;

    m_setup();

    if( m_ad_hoc_testresult.HasFatalFailure() )
    {
        return false;
    }

    if( CheckSetUpSkipped() )
    {
        return true;
    }

    {
        detail::iuStopWatch sw;
        sw.start();
        for( iuTestInfos::iterator it = m_testinfos.begin(), end=m_testinfos.end(); it != end; ++it )
        {
            // 実行
            if( !(*it)->Run() )
            {
                result = false;
            }
        }
        m_elapsedmsec = sw.stop();
    }

    m_teardown();
    if( m_ad_hoc_testresult.HasFatalFailure() )
    {
        return false;
    }
    return result;
}

IUTEST_IPP_INLINE bool TestCase::CheckSetUpSkipped()
{
    if(m_ad_hoc_testresult.Skipped())
    {
        for(iuTestInfos::iterator it = m_testinfos.begin(), end = m_testinfos.end(); it != end; ++it)
        {
            (*it)->skip();
        }
        return true;
    }
    return false;
}

IUTEST_IPP_INLINE void TestCase::clear()
{
    m_ad_hoc_testresult.Clear();
    for( iuTestInfos::iterator it = m_testinfos.begin(), end=m_testinfos.end(); it != end; ++it )
    {
        (*it)->clear();
    }
}

IUTEST_IPP_INLINE bool TestCase::filter()
{
    m_should_run_num = 0;
    m_disable_num = 0;
    for( iuTestInfos::iterator it = m_testinfos.begin(), end=m_testinfos.end(); it != end; ++it )
    {
        if( m_disable )
        {
            // DISABLE の伝搬
            (*it)->m_disable = true;
        }
        if( (*it)->is_disabled_test() )
        {
            ++m_disable_num;
        }
        if( (*it)->filter() )
        {
            ++m_should_run_num;
        }
    }
    return should_run();
}

IUTEST_IPP_INLINE int TestCase::reportable_test_count() const
{
    return detail::CountIfOverList(m_testinfos, &TestInfo::is_reportable);
}

IUTEST_IPP_INLINE int TestCase::failed_test_count() const
{
    if( !should_run() )
    {
        return 0;
    }
    // if( m_ad_hoc_testresult.Failed() )
    // {
    //     return reportable_test_count();
    // }
    return detail::CountIf(m_testinfos, IsFailedTest);
}

IUTEST_IPP_INLINE int TestCase::successful_test_count() const
{
    if( !should_run() )
    {
        return 0;
    }
    // if( m_ad_hoc_testresult.Failed() )
    // {
    //     return 0;
    // }
    return detail::CountIf(m_testinfos, IsSuccessfulTest);
}

IUTEST_IPP_INLINE int TestCase::skip_test_count() const
{
    if( !should_run() )
    {
        return total_test_count();
    }
    return detail::CountIf(m_testinfos, IsSkipTest);
}

IUTEST_IPP_INLINE int TestCase::reportable_skip_test_count() const
{
    if( !should_run() )
    {
        return reportable_test_count();
    }
    return detail::CountIf(m_testinfos, IsReportableSkipTest);
}

IUTEST_IPP_INLINE int TestCase::test_run_skipped_count() const
{
    if( !should_run() )
    {
        return 0;
    }
    return detail::CountIf(m_testinfos, IsRunSkippedTest);
}

IUTEST_IPP_INLINE int TestCase::reportable_test_run_skipped_count() const
{
    if( !should_run() )
    {
        return 0;
    }
    return detail::CountIf(m_testinfos, IsReportableRunSkippedTest);
}

IUTEST_IPP_INLINE int TestCase::reportable_disabled_test_count() const
{
    return detail::CountIf(m_testinfos, IsReportableDisabledTest);
}

}   // end of namespace iutest

#endif // INCG_IRIS_IUTEST_CASE_IPP_F57C9B7E_7CAA_4429_BE75_FCAAEED1B220_
