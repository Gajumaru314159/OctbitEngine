//***********************************************************
//! @file
//! @brief		タスクシステム
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/String/String.h>
#include <Framework/Core/Template/Utility/Function.h>

namespace ob::core {

	class Task {
	public:
		static Task Create(Action action) {

		}
		static Task Run(Action action) {
		}

		static Task Parallel(Func<void(s32)> action) {
		}

		static void WaitAll(Task& task1, Task& task2) {
		}

		// ContinueWith
		// Wait
		// WaitAny
		// 
	public:
		bool isCompleted()const {
		}
		void wait();
	};


	void TaskSample() {

		Task::Run([]() {});
		
		auto task1 = Task::Create([]() {});
		auto task2 = Task::Create([]() {});

		Task::WaitAll(task1,task2);



		// Animation更新
		Task::Parallel(
			[](s32 index) {
				// 並列処理
			}
		);

		// Physics更新
		Task::Parallel(
			[](s32 index) {
				// 並列処理
			}
		);

		// Transform更新
		Task::Parallel(
			[](s32 index) {
				// 並列処理
			}
		);

		// VFX更新
		Task::Parallel(
			[](s32 index) {
				// 並列処理
			}
		);

		// UI更新
		Task::Parallel(
			[](s32 index) {
				// 並列処理
			}
		);

		for (s32 i = 0; i < 16; ++i) {
			tf.emplace([i] { });
		}


	}


}