//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
/*
#include <Framework/Core/Job/JobSystem.h>
#include <Framework/Core/Job/JobExecutor.h>
#include <Framework/Core/Job/JobGroup.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief		同期コンテナ
	//! @details	要素の追加と削除をキューしておいて動機タイミングでまとめて処理する
	//!             コンテナ
	//@―---------------------------------------------------------------------------
	template<class T>
	class SyncContainer {
	public:

		void update() {
			ScopeLock lock(m_lock);
			// 追加
			for (auto& item : m_entryItems) {
				m_items.push_back(std::move(item));
			}
			// 削除
			for (auto itr = m_items.begin(); itr != m_items.end(); ) {
				if (m_leaveItems.count(itr->get())) {
					itr = m_items.erase(itr);
				} else {
					itr++;
				}
			}
		}

		void entry(UPtr<T>&& item) {
			ScopeLock lock(m_lock);
			m_entryItems.push_back(item);
		}

		void leave(const T* item) {
			ScopeLock lock(m_lock);
			m_leaveItems.emplace(item);
		}

		SpinLock            m_lock;
		Array<const T*>     m_items;
		Array<const T*>     m_entryItems;
		HashSet<const T*>   m_leaveItems;
	};

	class JobSystemImpl {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief 実行
		//@―---------------------------------------------------------------------------
		void execute() {
			m_executor.execute();
		}

		//@―---------------------------------------------------------------------------
		//! @brief 待機
		//@―---------------------------------------------------------------------------
		void wait() {
			m_executor.wait();
		}

		//@―---------------------------------------------------------------------------
		//! @brief JobGroup生成
		//@―---------------------------------------------------------------------------
		auto createJobGroup(StringView name)->JobGroup& {
			m_groups.entry(std::make_unique<JobGroup>(name));
		}

		//@―---------------------------------------------------------------------------
		//! @brief JobGroup生成
		//@―---------------------------------------------------------------------------
		auto createJob(StringView name, JobGroup& group, Action&& action)->JobGroup& {
			m_groups.entry(std::make_unique<Job>(name));
		}

	private:
		JobExecutor				m_executor;

		SyncContainer<JobGroup>	m_groups;
		SyncContainer<Job>		m_jobs;
	};



	JobSystem::JobSystem() {
	}
	void JobSystem::execute() { 
		m_impl->execute();
	}
	void JobSystem::wait() {
		m_impl->wait();
	}
	auto JobSystem::createJobGroup(StringView name)->JobGroup& {
		m_impl->createJobGroup(name);
	}

}
*/
