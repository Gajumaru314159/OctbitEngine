//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
/*
#include <Framework/Core/Job/JobGroup.h>
#include <Framework/Core/Job/JobExecutor.h>
#include <Framework/Core/Job/Job.h>
#include <Framework/Core/Job/JobSystem.h>

namespace ob {

	//@―---------------------------------------------------------------------------
	//! @brief コンストラクタ
	//@―---------------------------------------------------------------------------
	JobGroup::JobGroup(JobSystem& system, StringView name) 
		: m_system(system)
	{
		m_name = name;
	}

	//@―---------------------------------------------------------------------------
	//! @brief デストラクタ
	//@―---------------------------------------------------------------------------
	JobGroup::~JobGroup() {
		for (auto& job : m_jobs) {
			LOG_TRACE("未開放のジョブ [{}({})]",job->getName(),m_name);
		}
		OB_ASSERT(m_jobs.empty(), "未開放のジョブがあります");
	}

	//@―---------------------------------------------------------------------------
	//! @brief 名前を取得
	//@―---------------------------------------------------------------------------
	auto JobGroup::getName()const->const String& {
		return m_name;
	}

	//@―---------------------------------------------------------------------------
	//! @brief サブ JobGroup を生成
	//@―---------------------------------------------------------------------------
	auto JobGroup::createSub(StringView name)->Ref<JobGroup> {
		ScopeLock lock(m_entryLock);
		m_entrySubGroups.emplace_back(std::make_unique<JobGroup>(*this));
		return m_entrySubGroups.back().get();
	}

	//@―---------------------------------------------------------------------------
	//! @brief 子 JobGroup を生成
	//@―---------------------------------------------------------------------------
	auto JobGroup::createChild(StringView name)->Ref<JobGroup> {
		ScopeLock lock(m_entryLock);
		m_entryChildGroups.emplace_back(std::make_unique<JobGroup>(*this));
		return m_entryChildGroups.back().get();
	}

	//@―---------------------------------------------------------------------------
	//! @brief 子 JobGroup を追加
	//@―---------------------------------------------------------------------------
	void JobGroup::addChild(Ref<JobGroup>& group) {

		if (!group) {
			LOG_WARNING("空のJobGroupが登録されました");
			return;
		}
		if (std::find(m_childGroups.begin(), m_childGroups.end(), group) != m_childGroups.end()) {
			LOG_ERROR("JobGroup {}は既に登録されています。", group->getName());
			return;
		}
		m_childGroups.push_back(group);
		OB_NOTIMPLEMENTED();
	}

	//@―---------------------------------------------------------------------------
	//! @brief ジョブを追加
	//@―---------------------------------------------------------------------------
	auto JobGroup::addJob(StringView name, Action&& action)->JobHandle& {
		ScopeLock lock(m_entryLock);
		m_entryJobs.push_back(std::make_unique<Job>(*this,name,action));
	}

	//@―---------------------------------------------------------------------------
	//! @brief ジョブを削除
	//@―---------------------------------------------------------------------------
	void JobGroup::removeJob(Job& job) {
		ScopeLock lock(m_entryLock);
		m_leaveJobs.emplace(&job);
	}

	//@―---------------------------------------------------------------------------
	//! @brief 終端グループの数を数える
	//@―---------------------------------------------------------------------------
	s32 JobGroup::countLeafGroup()const {
		s32 count = 0;

		if (!m_childGroups.empty()) count++;

		for (auto& child : m_childGroups) {
			count += child->countLeafGroup();
		}

		return count;
	}

	//@―---------------------------------------------------------------------------
	//! @brief 更新
	//@―---------------------------------------------------------------------------
	void JobGroup::update(JobExecutor& executor) {
		ScopeLock lock(m_entryLock);

		// Job追加
		for (auto& job : m_entryJobs) {
			m_jobs.push_back(std::move(job));
		}
		// Job削除
		for (auto itr = m_jobs.begin(); itr != m_jobs.end(); ) {
			if (m_leaveJobs.count(itr->get())) {
				itr = m_jobs.erase(itr);
			} else {
				itr++;
			}
		}

		// JobGroup追加
		for (auto& group : m_entryChildGroups) {
			m_childGroups.push_back(std::move(group));
			group->addDeps(*this);
		}
		// JobGroup削除
		for (auto itr = m_childGroups.begin(); itr != m_childGroups.end(); ) {
			if (m_leaveGroups.count(itr->get())) {
				itr = m_childGroups.erase(itr);
			} else {
				itr++;
			}
		}

		// 依存解決
		for (auto& child : m_childGroups) {
			child->m_dependParentCount++;
		}

		m_dependSubCount = 0;
		for (auto& sub : m_subGroups) {
			m_dependSubCount += sub->countLeafGroup();
		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief ジョブを実行
	//@―---------------------------------------------------------------------------
	void JobGroup::execute(JobExecutor& executor) {

		// ジョブ実行
		s32 index = 0;
		while (true) {
			index = m_jobIndex.fetch_add(1);
			if (m_jobs.size() <= index)break;

			auto& job = m_jobs[index];
			
			if (job->isSuspended())continue;

			job->execute();
		}

		// JobGroup完了時
		// JobGroupの依存対決
		if (index == m_jobs.size()) {

			if (m_childGroups.empty()) {

				if (m_parent) {



				}

			} else {

			}


			if (0 < m_subGroups.size()) {

				// サブグループがあるなら開始
				for (auto& sub : m_subGroups) {
					OB_ASSERT_EXPR(sub);
					executor.entry(*sub);
				}

			} else if (0 < m_childGroups.size()) {

				// サブが無ければ子グループの参照数を変える
				for (auto& child : m_childGroups) {
					auto count = child->m_dependParentCount.fetch_sub(1);
					if (count != 1)continue;
					OB_ASSERT_EXPR(child);
					executor.entry(*child);
				}

			} else if(m_parent) {

				// 子が無ければ親の参照数を変える
				auto count = m_parent->m_dependSubCount.fetch_sub(1);
				if (count == 1) {

				}

			}

			// NextGroupはPrevGroupの全てのChildGroupに依存
			// 子JobGroupをエントリー
			for (auto& child : m_childGroups) {
				auto count = child->m_dependParentCount.fetch_sub(1);
				if (count != 1)continue;
				OB_ASSERT_EXPR(child);
				executor.entry(*child);
			}

		}

		if (m_parent) {
			m_parent->m_dependParentCount.fetch_sub(1);
		}
	}


	void JobGroup::requestRelease() {

	}

	void JobGroup::onLeaveSubGroup(JobExecutor& executor) {
		auto count = m_dependSubCount.fetch_sub(1);
		if (count <= 1) {
			for (auto& child : m_childGroups) {
				auto count = child->m_dependParentCount.fetch_sub(1);
				if (count != 1)continue;
				OB_ASSERT_EXPR(child);
				executor.entry(*child);
			}
		}
	}

}
*/