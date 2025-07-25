//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Thread/ThreadPool.h>

namespace ob::core {

	ThreadPool::ThreadPool() {
		resize(4);
	}

	ThreadPool::~ThreadPool() {
		join();
	}

	void ThreadPool::resize(s32 threadNum) {

		// 既存のスレッド終了
		join();

		// スレッド数の変更
		m_threads.clear();
		m_running = true;
		for (s32 i = 0; i < threadNum; ++i) {
			m_threads.emplace_back(Format("ThreadPool_{:2}", i), ThreadPriority::Level3, [this] { process(); });
		}

	}

	void ThreadPool::enqueue(Action&& action) {
		{
			std::lock_guard lock(m_mutex);
			m_queue.emplace_back(std::move(action));
		}
		m_condition.notify_one();
	}

	void ThreadPool::join() {
		{
			std::lock_guard lock(m_mutex);
			m_running = false;
			m_condition.notify_all();
		}
	}

	void ThreadPool::process() {
		while (true) {
			Action action;
			{
				std::unique_lock lock(m_mutex);
				m_condition.wait(lock, [this] { return !m_queue.empty() || !m_running; });

				if (m_running == false) {
					break;
				}

				action = std::move(m_queue.back());
				m_queue.pop_back();
			}
			// 処理
			action();
		}
	}

}