//***********************************************************
//! @file
//! @brief		ハードウェア共通デファイン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

	class Job;
	class JobGroup;

	//@―---------------------------------------------------------------------------
	//! @brief		ジョブ・ハンドル
	//! @details	ジョブに登録した関数がオブジェクトの解放後に実行されないように
	//!				するためのハンドルです。JobHandleの解放と同時にJobはサスペンド
	//!				状態に移行します。
	//@―---------------------------------------------------------------------------
	class JobHandle : Noncopyable, Nonmovable {
	public:
		JobHandle();
		JobHandle(Job&);
		~JobHandle();
		void release();
	private:
		Job* m_job = nullptr;
	};

	//@―---------------------------------------------------------------------------
	//! @brief		ジョブ・グループ・ハンドル
	//! @details	ジョブに登録した関数がオブジェクトの解放後に実行されないように
	//!				するためのハンドルです。JobHandleの解放と同時にJobはサスペンド
	//!				状態に移行します。
	//@―---------------------------------------------------------------------------
	class JobGroupHandle : Noncopyable, Nonmovable {
	public:
		JobGroupHandle();
		JobGroupHandle(JobGroup&);
		~JobGroupHandle();
		void release();
	private:
		JobGroup* m_group = nullptr;
	};

}