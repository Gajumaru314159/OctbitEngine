//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>

namespace ob::engine {

	namespace detail {

		//@―---------------------------------------------------------------------------
		//! @brief  モジュールのファクトリ基底クラス
		//@―---------------------------------------------------------------------------
		class ModuleFactoryBase {
		public:
			virtual IModule* construct(Engine&)const = 0;
			virtual size_t type()const = 0;
			virtual StringView name()const = 0;
		};

		//@―---------------------------------------------------------------------------
		//! @brief  モジュールのファクトリクラス
		//@―---------------------------------------------------------------------------
		template<class T,
			typename = std::enable_if_t<
			/**/std::is_base_of<IModule,T>::value &&		// IModuleの派生クラス
			/**/std::is_constructible<T, Engine&>::value	// 生成可能
			>
		>
		class ModuleFactory :public ModuleFactoryBase {
		public:
			ModuleFactory() {
				StringEncoder::Encode(typeid(T).name(), m_name);
			}
			IModule* construct(Engine& engine)const override {
				return new T(engine);
			}
			size_t type()const override {
				return typeid(T).hash_code();
			}
			StringView name()const override {
				return m_name;
			}
		private:
			String m_name;
		};
	}



	//@―---------------------------------------------------------------------------
	//! @brief		モジュール管理クラス
	//@―---------------------------------------------------------------------------
	class ModuleManager:Noncopyable {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		ModuleManager(Engine& engine):m_engine(engine){}

		//@―---------------------------------------------------------------------------
		//! @brief				生成可能なモジュールを追加
		//! 
		//! @tparam TModule		生成されるモジュール型
		//! @tparam TBase		基底型
		//@―---------------------------------------------------------------------------
		template<class TModule, class TBase = TModule>
		void add();

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

		HashMap<TypeHash, Array<UPtr<detail::ModuleFactoryBase>>> m_factoryMap;

		Array<UPtr<IModule>> m_modules;
		HashMap<TypeHash, size_t> m_indices;

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief				生成可能なモジュールを追加
	//! 
	//! @tparam TModule		生成されるモジュール型
	//! @tparam TBase		基底型
	//@―---------------------------------------------------------------------------
	template<class TModule, class TBase>
	void ModuleManager::add() {
		auto hash = typeid(TBase).hash_code();
		auto& factories = m_factoryMap[hash];
		auto& factory= factories.emplace_back(std::make_unique<detail::ModuleFactory<TModule>>());

		LOG_TRACE_EX("ModuleManager", "ModuleManagerに[{}]を追加", factory->name());
	}


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

		// ファクトリが存在するか
		auto& factories = m_factoryMap[hash];
		if (factories.empty()) {
			return false;
		}

		// 生成
		detail::ModuleFactoryBase* pFactory = nullptr;
		IModule* pModule = nullptr;
		for (auto& factory : factories) {
			if (auto tmp = factory->construct(m_engine)) {
				if (tmp->isValid() == false) {
					delete tmp;
				} else {
					pFactory = factory.get();
					pModule = tmp;
					break;
				}
			}
		}

		// 登録
		if (pModule && pFactory) {
			auto index = m_modules.size();
			m_indices[hash] = m_indices[pFactory->type()] = index;
			m_modules.emplace_back(pModule);

			LOG_TRACE_EX("ModuleManager", "Module[{}]を生成", pFactory->name());

			return true;
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