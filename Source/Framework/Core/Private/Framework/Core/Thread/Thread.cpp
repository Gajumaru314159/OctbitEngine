//***********************************************************
//! @file
//! @brief		スレッド
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Thread/Thread.h>
#include <Framework/Core/String/StringEncoder.h>
#include <functional>

#ifdef OS_WINDOWS
#include <Windows.h>
#include <combaseapi.h>
namespace ob::core {

	class ThreadImpl {
	public:
		std::thread th;
	};

}
#else
#pragma error("Thread is not supported in this platform.")
#endif

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief				デフォルトコンストラクタ
	//@―---------------------------------------------------------------------------
	Thread::Thread() {

	}

	//@―---------------------------------------------------------------------------
	//! @brief				コンストラクタ
	//! 
	//! @param name			スレッド名
	//! @param desc			定義
	//! @param entryPoint	実行する関数オブジェクト
	//@―---------------------------------------------------------------------------
	Thread::Thread(StringView name, ThreadDesc desc, const Func<void()>& entryPoint) 
	{
		m_impl->th = std::thread(entryPoint);
#ifdef OS_WINDOWS
		WString wname;
		StringEncoder::Encode(name, wname);
		auto hr = ::SetThreadDescription(m_impl->th.native_handle(), wname.c_str());
		if (FAILED(hr)) {
			LOG_WARNING("スレッド名の設定に失敗 [{}]",name);
		}
#else
		static_assert(false, "Thread::Thread()が実装されていません。");
#endif
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
}