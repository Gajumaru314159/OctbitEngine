//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>
#include <Framework/Engine/ModuleFactory.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief		モジュール管理クラス
	//@―---------------------------------------------------------------------------
	class ModuleManager:public Singleton<ModuleManager> {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		ModuleManager(Engine& engine):m_engine(engine){}

		//@―---------------------------------------------------------------------------
		//! @brief				モジュール生成
		//! 
		//! @details			生成可能なモジュールが複数ある場合、指定した優先度順に生成を試みます。
		//! @retval true		成功
		//! @retval false		失敗
		//@―---------------------------------------------------------------------------
		template<class TModule>
		bool create();

		//@―---------------------------------------------------------------------------
		//! @brief				モジュールを取得
		//! 
		//! @deitals			add()でTModule、またはTBaseで指定した型でのみ取得できます。
		//@―---------------------------------------------------------------------------
		template<class TModule>
		TModule* get()const;

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
		template<class TFunc=void(IModule&)>
		void visit(TFunc);

	private:

		using TypeHash = size_t;

		Engine& m_engine;

		Array<UPtr<IModule>> m_modules;
		HashMap<TypeHash, size_t> m_indices;

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief				モジュール生成
	//! 
	//! @details			生成可能なモジュールが複数ある場合、指定した優先度順に生成を試みます。
	//@―---------------------------------------------------------------------------
	template<class TModule>
	bool ModuleManager::create() {

		auto hash = typeid(TModule).hash_code();

		// 登録済みかチェック
		if (auto found = get<TModule>()) {
			return true;
		}

		// 生成
		auto [pModule, baseTypeHash] = internal::ModuleFactory::Get().create<TModule>(m_engine);
		if (pModule) {

			auto index = m_modules.size();
			m_indices[hash] = m_indices[baseTypeHash] = index;
			m_modules.emplace_back(pModule);

		}

		return false;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  モジュールを取得
	//@―---------------------------------------------------------------------------
	template<class TModule>
	inline TModule* ModuleManager::get()const {

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
	inline bool ModuleManager::has()const {
		return get<TModule>() != nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief	モジュール走査
	//@―---------------------------------------------------------------------------
	template<class TFunc>
	inline void ModuleManager::visit(TFunc func) {
		for (auto& m : m_modules) {
			func(*m.get());
		}
	}


	//! @endcond
}// namespcae ob