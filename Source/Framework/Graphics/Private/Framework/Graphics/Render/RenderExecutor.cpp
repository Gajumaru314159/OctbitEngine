//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderExecutor.h>

namespace ob::graphics {

    RenderExecutor::RenderExecutor(const RenderExecutorDesc& desc) {
        for (s32 i = 0; i < desc.threadNum; ++i) {
            m_threads.emplace_back(
                Format(TC("RenderThread{}"),i),
                ThreadPriority::Level5,
                [this, i] {execute(i); }
            );
        }
    }

    RenderExecutor::~RenderExecutor() {

    }

    void RenderExecutor::wakeup() {
        // TODO 終了待ち

        m_executeCount = m_threads.size();
        m_conditionVariable.notify_all();
    }

    void RenderExecutor::wait() {

        std::unique_lock<std::mutex> lock(m_mutex);
        m_waitCond.wait(lock, [this] {return m_executeCount == 0; });
    }


    void RenderExecutor::execute(s32 threadIndex) {
        while (true) {
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_conditionVariable.wait(lock);
            }

            // 描画処理実行

            if (m_executeCount.fetch_sub(1) == 1) {
                m_waitCond.notify_one();
            }

        }
    }

}