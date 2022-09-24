﻿//***********************************************************
//! @file
//! @brief		エンジン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/ModuleFactory.h>

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
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		Engine();

		//@―---------------------------------------------------------------------------
		//! @brief		デストラクタ
		//@―---------------------------------------------------------------------------
		~Engine();

		//@―---------------------------------------------------------------------------
		//! @brief				モジュール取得
		//! 
		//! @details			生成可能なモジュールが複数ある場合、指定した優先度順に生成を試みます。
		//! @return				モジュールのポインタ。生成できるモジュールがなかった場合はnullptrを返す。
		//@―---------------------------------------------------------------------------
		template<class TModule>
		TModule* get();

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

	private:

		//@―---------------------------------------------------------------------------
		//! @brief				モジュール取得(内部)
		//@―---------------------------------------------------------------------------
		template<class TModule>
		TModule* getInternal()const;


	private:

		using TypeHash = size_t;

		Array<UPtr<IModule>> m_modules;
		HashMap<TypeHash, size_t> m_indices;

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

		auto hash = typeid(TModule).hash_code();

		// 登録済みかチェック
		if (auto found = this->getInternal<TModule>()) {
			return found;
		}

		// 生成
		auto [pModule, baseTypeHash] = internal::ModuleFactory::Get().create<TModule>(*this);
		if (pModule) {

			auto index = m_modules.size();
			m_indices[hash] = m_indices[baseTypeHash] = index;
			m_modules.emplace_back(pModule);

		}

		return pModule;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  モジュール取得(内部)
	//@―---------------------------------------------------------------------------
	template<class TModule>
	inline TModule* Engine::getInternal()const {

		TypeHash hash = typeid(TModule).hash_code();

		auto found = m_indices.find(hash);
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