//***********************************************************
//! @file
//! @brief		スピンロック
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Thread/SpinLock.h>
#include <Framework/core/Thread/Thread.h>
#include <atomic>

#ifdef OB_DEBUG_SPIN_LOCK
#	define OB_DEBUG_SPIN_LOCK_CONTEX(context)	context
#else
#	define OB_DEBUG_SPIN_LOCK_CONTEX(contex)	/**/
#endif

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class SpinLockImpl {
	public:
		std::atomic<bool> m_lock;
		OB_DEBUG_SPIN_LOCK_CONTEX(
			std::atomic<u32> m_threadId{ 0 };
		)
	};

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	SpinLock::SpinLock() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	SpinLock::~SpinLock() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  ロックを取得する
	//@―---------------------------------------------------------------------------
	void SpinLock::lock() {
		OB_DEBUG_SPIN_LOCK_CONTEX(
			u32 id = m_impl->m_threadId.load();
			assert(id != Thread::GetCurrentThreadId());
		)
		
		while (true) {
			if (!m_impl->m_lock.exchange(true, std::memory_order_acquire)) {
				break;
			}
			while (m_impl->m_lock.load(std::memory_order_relaxed)) {
				_mm_pause();
			}
		}

		OB_DEBUG_SPIN_LOCK_CONTEX(
			m_impl->m_threadId.store(Thread::GetCurrentThreadId());
		)
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ロックを手放す
	//@―---------------------------------------------------------------------------
	void SpinLock::unlock() {
		OB_DEBUG_SPIN_LOCK_CONTEX(
			m_impl->m_threadId.store(0);
		)
		m_impl->m_lock.store(false,std::memory_order_release);
	}

}

#undef OB_DEBUG_SPIN_LOCK_CONTEX