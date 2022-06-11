//***********************************************************
//! @file
//! @brief		スレッド
//! @author		Gajumaru
//***********************************************************
#include <Framework/core/Thread/Thread.h>
#include <functional>

#ifdef OS_WINDOWS
#include <Windows.h>
#include <combaseapi.h>
namespace ob::core {

	class ThreadImpl {
	public:
		std::thread th;
		//HANDLE handle = NULL;
		//std::function<void()> entryPoint = nullptr;
	};

}
#endif

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief				コンストラクタ
	//! 
	//! @param name			スレッド名
	//! @param desc			定義
	//! @param entryPoint	実行する関数オブジェクト
	//@―---------------------------------------------------------------------------
	Thread::Thread(StringView name, ThreadDesc desc, const std::function<void()>& entryPoint) 
	{
		m_impl->th = std::thread(entryPoint);
	}

	//@―---------------------------------------------------------------------------
	//! @brief				デストラクタ
	//! 
	//! @details			スレッドが終了するまで待機
	//@―---------------------------------------------------------------------------
	Thread::~Thread() {
		join();
	}

	//@―---------------------------------------------------------------------------
	//! @brief				スレッドが終了するまで待機する
	//@―---------------------------------------------------------------------------
	void Thread::join() {
		m_impl->th.join();
	}

	//@―---------------------------------------------------------------------------
	//! @brief				スレッドの実行を他スレッドに譲る
	//@―---------------------------------------------------------------------------
	void Thread::YieldThread() {
#ifdef OS_WINDOWS
		::SwitchToThread();
#else
		static_assert(false,"Thread::YieldThread()が実装されていません。");
#endif
	}

	//@―---------------------------------------------------------------------------
	//! @brief				スレッドの実行を他スレッドに譲る
	//@―---------------------------------------------------------------------------
	void Thread::Sleep(u32 milliSeconds) {
#ifdef OS_WINDOWS
		::Sleep(milliSeconds);
#else
		static_assert(false, "Thread::Sleep()が実装されていません。");
#endif
	}

	//@―---------------------------------------------------------------------------
	//! @brief				現在のスレッドIDを取得
	//@―---------------------------------------------------------------------------
	u32 Thread::GetCurrentThreadId() {
#ifdef OS_WINDOWS
		return static_cast<u32>(::GetCurrentThreadId());
#else
		static_assert(false, "Thread::GetCurrentThreadId()が実装されていません。");
#endif
	}

	//@―---------------------------------------------------------------------------
	//! @brief				スレッドを実行しているCPUのコア番号を取得
	//@―---------------------------------------------------------------------------
	u32 Thread::GetCurrentCpuCore() {
#ifdef OS_WINDOWS
		return static_cast<u32>(::GetCurrentProcessorNumber());
#else
		static_assert(false, "Thread::GetCurrentCpuCore()が実装されていません。");
#endif

	}
}// namespace ob