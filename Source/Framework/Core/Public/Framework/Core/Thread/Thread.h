﻿//***********************************************************
//! @file
//! @brief		スレッド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/String/String.h>
#include <Framework/Core/Utility/Noncopyable.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  スレッド優先順序
	//@―---------------------------------------------------------------------------
	enum class ThreadPriority : u32 {
		Level1,	//!< Critical
		Level2,	//!< Highest
		Level3,	//!< AboveNormal
		Level4,	//!< Normal
		Level5,	//!< BelowNormal
		Level6,	//!< Lowest
	};

	//@―---------------------------------------------------------------------------
	//! @brief  スレッド優先順序
	//@―---------------------------------------------------------------------------
	struct ThreadDesc {
		u32 affinity = 0xFFFFFFFF;
		u32 stackSize = 64 * 1024;
		ThreadPriority priority = ThreadPriority::Level3;
	public:
		ThreadDesc() = default;
		ThreadDesc(ThreadPriority p) :priority(p){}
	};

	//@―---------------------------------------------------------------------------
	//! @brief  スレッド
	//@―---------------------------------------------------------------------------
	class Thread : Noncopyable {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief				デフォルトコンストラクタ
		//@―---------------------------------------------------------------------------
		Thread();

		//@―---------------------------------------------------------------------------
		//! @brief				コンストラクタ
		//! 
		//! @param name			スレッド名
		//! @param entryPoint	実行する関数オブジェクト
		//@―---------------------------------------------------------------------------
		Thread(StringView name, const std::function<void()>& entryPoint)
			: Thread(name, ThreadDesc{},entryPoint){}

		//@―---------------------------------------------------------------------------
		//! @brief				コンストラクタ
		//! 
		//! @param name			スレッド名
		//! @param priority		実行優先順序
		//! @param entryPoint	実行する関数オブジェクト
		//@―---------------------------------------------------------------------------
		Thread(StringView name, ThreadPriority priority, const std::function<void()>& entryPoint)
			: Thread(name, ThreadDesc(priority), entryPoint) {}

		//@―---------------------------------------------------------------------------
		//! @brief				コンストラクタ
		//! 
		//! @param name			スレッド名
		//! @param desc			定義
		//! @param entryPoint	実行する関数オブジェクト
		//@―---------------------------------------------------------------------------
		Thread(StringView name, ThreadDesc desc, const std::function<void()>& entryPoint);

		//@―---------------------------------------------------------------------------
		//! @brief				デストラクタ
		//! 
		//! @details			スレッドが終了するまで待機します。
		//@―---------------------------------------------------------------------------
		~Thread();

		//@―---------------------------------------------------------------------------
		//! @brief				スレッドが終了するまで待機する
		//@―---------------------------------------------------------------------------
		void join();

	public:

		//@―---------------------------------------------------------------------------
		//! @brief				スレッドの実行を他スレッドに譲る
		//@―---------------------------------------------------------------------------
		static void YieldThread();

		//@―---------------------------------------------------------------------------
		//! @brief				スレッドの実行を他スレッドに譲る
		//@―---------------------------------------------------------------------------
		static void Sleep(u32 milliSeconds);

		//@―---------------------------------------------------------------------------
		//! @brief				現在のスレッドIDを取得
		//@―---------------------------------------------------------------------------
		static u32 GetCurrentThreadId();

		//@―---------------------------------------------------------------------------
		//! @brief				スレッドを実行しているCPUのコア番号を取得
		//@―---------------------------------------------------------------------------
		static u32 GetCurrentCpuCore();


	private:

		Pimpl<class ThreadImpl> m_impl;

	};

}// namespcae ob