//***********************************************************
//! @file
//! @brief		ミューテックス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Thread/Mutex.h>
#include <mutex>
#include <atomic>
#include <Framework/Core/Thread/Thread.h>

#ifdef OB_DEBUG_MUTEX
#	define OB_DEBUG_MUTEX_CONTEX(context)	context
#else
#	define OB_DEBUG_MUTEX_CONTEX(contex)	/**/
#endif

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class MutexImpl {
	public:
		std::mutex m_mutex;
		OB_DEBUG_MUTEX_CONTEX(
			std::atomic<u32> m_threadId{ 0 };
		)
	};

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	Mutex::Mutex() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	Mutex::~Mutex() {

	}


	//@―---------------------------------------------------------------------------
	//! @brief  ロックを取得する
	//@―---------------------------------------------------------------------------
	void Mutex::lock() {
		OB_DEBUG_MUTEX_CONTEX(
			u32 id = m_impl->m_threadId.load();
			assert(id != Thread::GetCurrentThreadId());
		)

		m_impl->m_mutex.lock();

		OB_DEBUG_MUTEX_CONTEX(
			m_impl->m_threadId.store(Thread::GetCurrentThreadId());
		)
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ロックの取得を試みる
	//@―---------------------------------------------------------------------------
	bool Mutex::try_lock() {
		return m_impl->m_mutex.try_lock();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ロックを手放す
	//@―---------------------------------------------------------------------------
	void Mutex::unlock() {
		OB_DEBUG_MUTEX_CONTEX(
			m_impl->m_threadId.store(0);
		)
		m_impl->m_mutex.unlock();
	}

}// namespcae ob::core

#undef OB_DEBUG_MUTEX_CONTEX