//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Thread/Thread.h>
#include <Framework/Core/Utility/Singleton.h>
#include <Framework/RHI/Forward.h>


namespace ob::graphics {
       
    struct RenderExecutorDesc {
        s32 threadNum = 1;
    };

    //@―---------------------------------------------------------------------------
    //! @brief RenderThreadを実行するクラス 
    //@―---------------------------------------------------------------------------
    class RenderExecutor {
    public:

        RenderExecutor(const RenderExecutorDesc&);
        ~RenderExecutor();

        void wakeup();
        void wait();

    private:

        void execute(s32 threadIndex);

    private:
        std::mutex m_mutex;
        std::condition_variable m_conditionVariable;
        std::atomic_int m_executeCount;
        Array<Thread> m_threads;

        std::condition_variable m_waitCond;
    };


}