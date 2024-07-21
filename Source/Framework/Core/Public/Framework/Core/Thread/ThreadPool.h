//***********************************************************
//! @file
//! @brief		スレッド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/String/String.h>
#include <Framework/Core/Thread/Thread.h>
#include <Framework/Core/Utility/Noncopyable.h>
#include <Framework/Core/Utility/Singleton.h>

namespace ob::core {

	class ThreadPool : public Singleton<ThreadPool> {
	public:

		ThreadPool();
		~ThreadPool();

		void resize(s32 threadNum);
		void enqueue(Action&& action);

	private:

		void join();
		void process();

	private:
		std::mutex m_mutex;
		std::condition_variable m_condition;
		std::atomic_bool m_running = false;
		Deque<Action> m_queue;

		Vector<Pimpl<Thread>> m_threads;
	};

}