//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Thread/SpinLock.h>
#include <Framework/Core/Thread/Thread.h>
#include <atomic>

namespace ob::core {

	//! @brief  ロックを取得する
	void SpinLock::Impl::lock() {

		OB_DEBUG_SPIN_LOCK_CONTEX(
			u32 id = m_threadId.load();
			assert(id != Thread::GetCurrentThreadId());
		)

		while (true) {
			if (!m_lock.exchange(true, std::memory_order_acquire)) {
				break;
			}
			while (m_lock.load(std::memory_order_relaxed)) {
#ifdef OS_WINDOWS
				_mm_pause();
#else
				std::this_thread::yield();
#endif
			}
		}

		OB_DEBUG_SPIN_LOCK_CONTEX(
			m_threadId.store(Thread::GetCurrentThreadId());
		)
	}


	//! @brief  ロックを手放す
	void SpinLock::Impl::unlock() {
		OB_DEBUG_SPIN_LOCK_CONTEX(
			m_threadId.store(0);
		)
		m_lock.store(false,std::memory_order_release);
	}



	//! @brief  コンストラクタ
	SpinLock::SpinLock() {

	}


	//! @brief  デストラクタ
	SpinLock::~SpinLock() {

	}


	//! @brief  ロックを取得する
	void SpinLock::lock() {
		m_impl.lock();
	}


	//! @brief  ロックを手放す
	void SpinLock::unlock() {
		m_impl.unlock();
	}

}

#undef OB_DEBUG_SPIN_LOCK_CONTEX