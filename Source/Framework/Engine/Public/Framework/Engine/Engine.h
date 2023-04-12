﻿//***********************************************************
//! @file
//! @brief		エンジン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/EngineConfig.h>
#include <Framework/Core/Reflection/TypeId.h>
#include <Framework/Core/Utility/DI.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  エンジン
	//@―---------------------------------------------------------------------------
	class Engine :Noncopyable {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief				コンストラクタ
		//@―---------------------------------------------------------------------------
		Engine(EngineConfig&&,ServiceInjector& injector);

		//@―---------------------------------------------------------------------------
		//! @brief				デストラクタ
		//@―---------------------------------------------------------------------------
		~Engine();

		//@―---------------------------------------------------------------------------
		//! @brief				起動
		//@―---------------------------------------------------------------------------
		void startup();

		//@―---------------------------------------------------------------------------
		//! @brief				モジュール取得
		//! 
		//! @details			生成可能なモジュールが複数ある場合、指定した優先度順に生成を試みます。
		//! @return				モジュールのポインタ。生成できるモジュールがなかった場合はnullptrを返す。
		//@―---------------------------------------------------------------------------
		template<class TModule>
		TModule* get();

		//@―---------------------------------------------------------------------------
		//! @brief				コンフィグ取得
		//@―---------------------------------------------------------------------------
		bool update();

		//@―---------------------------------------------------------------------------
		//! @brief				コンフィグ取得
		//@―---------------------------------------------------------------------------
		EngineConfig& config() { return m_config; }

	private:


	private:

		EngineConfig m_config;
		ServiceContainer m_container;

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief				モジュール取得
	//! 
	//! @details			生成可能なモジュールが複数ある場合、指定した優先度順に生成を試みます。
	//! @return				モジュールのポインタ。生成できるモジュールがなかった場合はnullptrを返す。
	//@―---------------------------------------------------------------------------
	template<class TModule>
	inline TModule* Engine::get() {

		if (auto service = m_container.get<TModule>()) {
			return service;
		}
		return nullptr;
	}

	//! @endcond
}

//@―---------------------------------------------------------------------------
//! @brief	グローバルエンジンオブジェクト
//@―---------------------------------------------------------------------------
extern ob::engine::Engine* GEngine;