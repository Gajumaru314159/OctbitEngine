//***********************************************************
//! @file
//! @brief		ハードウェア共通デファイン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Utility/Singleton.h>
#include <Framework/Core/Utility/Ref.h>

namespace ob::core {

	class JobGroup;
	class Job;

	class JobSystem : public Singleton<JobSystem> {
	public:
		JobSystem();
		auto getRootGroup() -> JobGroup&;
		void execute();
		void wait();
	private:
		Ref<JobGroup>   m_rootGroup;

		Array<SPtr<JobGroup>>	m_groups;
		Array<SPtr<Job>>		m_jobs;

		Pimpl<class JobExecutor> m_executor;
	};

}