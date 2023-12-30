//***********************************************************
//! @file
//! @brief		ハードウェア共通デファイン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Thread/Thread.h>
#include <Framework/Core/Job/JobGroup.h>

namespace ob::core {

    class JobExecutor {
    public:
        JobExecutor();
        ~JobExecutor();
        void execute();
        void wait();
        void entry(JobGroup& group);
    private:
        void resizeThread(s32 num);
        void executeThread(s32 threadIndex);
    private:
        struct JobThread {
            Thread      thread;
            s32         index;
        };
    private:

        std::mutex              m_mutex;
        std::condition_variable m_condition;

        std::mutex              m_executeMutex;
        std::condition_variable m_executeCondition;
        std::atomic<bool>       m_executing;

        Atomic<int>             m_jobCount = 0;
        Atomic<bool>            m_completeFlag = false;

        Atomic<bool>            m_stopFlag = false;

        Array<JobGroup*>        m_groups;

        Array<JobGroup*>        m_groupQueue;
        Atomic<s32>             m_groupIndex;
        Atomic<s32>             m_groupSize;

        Array<JobThread>        m_threads;

        // インスタンス
        Array<UPtr<JobGroup>>   m_groups;
        Array<UPtr<Job>>        m_jobs;
    };

}