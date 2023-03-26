﻿//***********************************************************
//! @file
//! @brief		エンジン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/ModuleFactory.h>
#include <Framework/Engine/EngineConfig.h>
#include <Framework/Core/Reflection/TypeId.h>
#include <Framework/Core/Utility/DI.h>

namespace ob::engine {

	class IModule;

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
		Engine(EngineConfig&&);

		//@―---------------------------------------------------------------------------
		//! @brief				コンストラクタ
		//@―---------------------------------------------------------------------------
		Engine(EngineConfig&&,const DependencyGraph& graph);

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
		//! @brief				モジュール取得
		//! 
		//! @details			生成可能なモジュールが複数ある場合、指定した優先度順に生成を試みます。
		//!						取得可能なモジュールがなかった場合エラーを出してプログラムを終了します。
		//@―---------------------------------------------------------------------------
		template<class TModule>
		TModule& ref();

		//@―---------------------------------------------------------------------------
		//! @brief				モジュールを持っているか
		//! 
		//! @deitals			add()でTModule、またはTBaseで指定した型でのみ取得できます。
		//@―---------------------------------------------------------------------------
		template<class TModule>
		bool has()const;

		//@―---------------------------------------------------------------------------
		//! @brief				モジュール走査
		//@―---------------------------------------------------------------------------
		template<class TFunc = void(IModule&)>
		void visit(TFunc);

		//@―---------------------------------------------------------------------------
		//! @brief				コンフィグ取得
		//@―---------------------------------------------------------------------------
		bool update();

		//@―---------------------------------------------------------------------------
		//! @brief				コンフィグ取得
		//@―---------------------------------------------------------------------------
		EngineConfig& config() { return m_config; }

	private:

		//@―---------------------------------------------------------------------------
		//! @brief				モジュール取得(内部)
		//@―---------------------------------------------------------------------------
		template<class TModule>
		TModule* getInternal()const;


	private:

		Array<UPtr<IModule>> m_modules;
		HashMap<TypeId, size_t> m_indices;
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

		auto typeId = TypeId::Get<TModule>();

		// 登録済みかチェック
		if (auto found = this->getInternal<TModule>()) {
			return found;
		}

		// 生成
		auto [pModule, baseTypeHash] = internal::ModuleFactory::Get().create<TModule>();
		if (pModule) {

			auto index = m_modules.size();
			m_indices[typeId] = m_indices[baseTypeHash] = index;
			m_modules.emplace_back(pModule);

		}

		return pModule;
	}

	//@―---------------------------------------------------------------------------
	//! @brief				モジュール取得
	//! 
	//! @details			生成可能なモジュールが複数ある場合、指定した優先度順に生成を試みます。
	//! @return				モジュールのポインタ。生成できるモジュールがなかった場合はnullptrを返す。
	//@―---------------------------------------------------------------------------
	template<class TModule>
	inline TModule& Engine::ref() {
		auto pModule = get<TModule>();
		OB_ASSERT(pModule, "モジュールが登録されていません。[{}]", TypeId::Get<TModule>().name());
		return *pModule;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  モジュール取得(内部)
	//@―---------------------------------------------------------------------------
	template<class TModule>
	inline TModule* Engine::getInternal()const {

		TypeId typeId = TypeId::Get<TModule>();

		auto found = m_indices.find(typeId);
		if (found != m_indices.end()) {
			return reinterpret_cast<TModule*>(m_modules[found->second].get());
		}
		return nullptr;

	}


	//@―---------------------------------------------------------------------------
	//! @brief  モジュールを持っているか
	//@―---------------------------------------------------------------------------
	template<class TModule>
	inline bool Engine::has()const {
		return get<TModule>() != nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief	モジュール走査
	//@―---------------------------------------------------------------------------
	template<class TFunc>
	inline void Engine::visit(TFunc func) {
		for (auto& m : m_modules) {
			func(*m.get());
		}
	}


	//! @endcond
}// namespcae ob

//@―---------------------------------------------------------------------------
//! @brief	グローバルエンジンオブジェクト
//@―---------------------------------------------------------------------------
extern ob::engine::Engine* GEngine;