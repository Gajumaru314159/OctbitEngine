//***********************************************************
//! @file
//! @brief		ハードウェア共通デファイン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/String/StringView.h>
#include <Framework/Core/Utility/Singleton.h>
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Core/Utility/Pimpl.h>

namespace ob::core {

	class JobGroup;
	class Job;

	class JobSystem : public Singleton<JobSystem> {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief コンストラクタ
		//@―---------------------------------------------------------------------------
		JobSystem();

		//@―---------------------------------------------------------------------------
		//! @brief ジョブの実行を開始
		//@―---------------------------------------------------------------------------
		void execute();

		//@―---------------------------------------------------------------------------
		//! @brief ジョブの実行完了を待機
		//@―---------------------------------------------------------------------------
		void wait();

		//@―---------------------------------------------------------------------------
		//! @brief JobGroup を作成
		//@―---------------------------------------------------------------------------
		auto createJobGroup(StringView name)->JobGroup&;

		//@―---------------------------------------------------------------------------
		//! @brief Job を作成
		//@―---------------------------------------------------------------------------
		auto createJob(StringView name, JobGroup& group, Action&& action);

	private:
		Pimpl<class JobSystemImpl> m_impl;
	};

}