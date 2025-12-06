//***********************************************************
//! @file
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

        Vector<JobGroup*>        m_groups;

        Vector<JobGroup*>        m_groupQueue;
        Atomic<s32>             m_groupIndex;
        Atomic<s32>             m_groupSize;

        Vector<JobThread>        m_threads;

        // インスタンス
        Vector<UPtr<JobGroup>>   m_groups;
        Vector<UPtr<Job>>        m_jobs;
    };

}