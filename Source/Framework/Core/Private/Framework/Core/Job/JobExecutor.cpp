//***********************************************************
//! @file
//! @brief		ハードウェア共通デファイン
//! @author		Gajumaru
//***********************************************************
/*
#include <Framework/Core/Job/JobExecutor.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief コンストラクタ
	//@―---------------------------------------------------------------------------
	JobExecutor::JobExecutor() {
		resizeThread(1);
	}

	//@―---------------------------------------------------------------------------
	//! @brief デストラクタ
	//@―---------------------------------------------------------------------------
	JobExecutor::~JobExecutor() {

		// 全てのワーカースレッドに停止信号を送る
		m_stopFlag.store(true);

		// 全てのワーカースレッドを起こす
		m_condition.notify_all();

		// 全てのワーカースレッドが終了するのを待つ
		for (auto& thread : m_threads) {
			thread.thread.join();
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief スレッド数変更
	//@―---------------------------------------------------------------------------
	void JobExecutor::resizeThread(s32 num) {
		m_threads.clear();
		for (s32 i = 0; i < num; ++i) {
			m_threads.emplace_back(
				Thread(
					Format(TC("JobThread{:02}"), i),
					[this, i]() { executeThread(i); }
				),
				0
			);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief ジョブ実行
	//@―---------------------------------------------------------------------------
	void JobExecutor::executeThread(s32 threadIndex) {
		while (true) {

			auto& index = m_threads[threadIndex].index;

			// 待機JobGroupがない場合は待機
			if (m_groupIndex <= index) {
				std::unique_lock lock(m_mutex);
				m_condition.wait(lock, [this, &index, threadIndex] { return index < m_groupIndex || m_stopFlag; });
			}

			if (index < m_groupQueue.size()) {

				// 実行
				OB_ASSERT_EXPR(m_groupQueue[index]);
				m_groupQueue[index]->execute(*this);
				index++;

			} else {

				// 実行完了
				index = 0;
				m_executing = true;
				m_executeCondition.notify_one();

				if (m_stopFlag) break;

			}

		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief JobGroup 投入
	//@―---------------------------------------------------------------------------
	void JobExecutor::entry(JobGroup& group) {

		m_groupQueue.push_back(&group);
		m_groupSize++;
		m_condition.notify_one();

	}

	//@―---------------------------------------------------------------------------
	//! @brief 開始
	//@―---------------------------------------------------------------------------
	void JobExecutor::execute() {

		// 実行中であれば待機
		wait();

		// TODO 追加予定の解決
		for (auto& group : m_groups) {

		}

		m_groupQueue.clear();
		m_groupQueue.resize(m_groups.size());

		// 依存のないJobGroupを投入
		for (auto& group : m_groups) {
			OB_ASSERT_EXPR(group);
			if (group->getParent() == nullptr) {
				entry(*group);
			}
		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief 実行完了待機
	//@―---------------------------------------------------------------------------
	void JobExecutor::wait() {
		if (m_executing) {
			std::unique_lock lock(m_executeMutex);
			m_executeCondition.wait(lock, [this] { return !m_executing; });
		}
	}

}
*/