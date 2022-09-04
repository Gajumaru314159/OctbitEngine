//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/String/Name.h>

namespace ob::engine {

	class IModule;

	class ModuleConstructorBase {
	public:
		virtual IModule* construct() {};
	};

	template<class T, std::enable_if<std::is_base_of_v<T, IModule>>>
	class ModuleConstructor :public ModuleConstructorBase {
	public:
		IModule* construct()override {
			return ob::construct_at<T>();
		}
	};

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class ModuleManager {
    public:

		template<class TModule,class TBase=TModule>
		void add() {
			auto hash = typeid(TBase).hash_code();
			auto& factoryArray = m_factories[hash];
			factoryArray.push_back(std::make_unique<ModuleConstructor<TModule>>());
		}

		template<class TModule>
		void create() {
			auto hash = typeid(TBase).hash_code();

		}


		//@―---------------------------------------------------------------------------
		//! @brief  設定追加
		//@―---------------------------------------------------------------------------
		template<class TSettings>
		void addSettings(TSettings&& settings);

		//@―---------------------------------------------------------------------------
		//! @brief  設定追加
		//@―---------------------------------------------------------------------------
		template<class TSettings>
		const TSettings& getSettings();

		//@―---------------------------------------------------------------------------
		//! @brief  モジュールを追加
		//@―---------------------------------------------------------------------------
		template<class TModule, class TSettings = TModule::Settings>
		TModule* add();

		//@―---------------------------------------------------------------------------
		//! @brief  モジュールを取得
		//@―---------------------------------------------------------------------------
		template<class TModule>
		TModule* get()const;

	private:

		using TypeHash = size_t;

		HashMap<TypeHash, Array<UPtr<ModuleConstructorBase>>> m_factories;


		Array<UPtr<IModule>> m_modules;
		HashMap<TypeHash, size_t> m_indices;
		HashMap<TypeHash, std::any> m_settings;
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

	//@―---------------------------------------------------------------------------
	//! @brief  設定追加
	//@―---------------------------------------------------------------------------
	template<class TSettings>
	void ModuleManager::addSettings(TSettings&& settings) {
		m_settings[typeid(TSettings).hash_code()] = std::forward(settings);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  モジュールを追加
	//@―---------------------------------------------------------------------------
	template<class TModule, class TSettings>
	inline TModule* ModuleManager::add() {

		// 登録済みかチェック
		if (auto found = get<TModule>()) {
			return found;
		}

		// モジュール名取得
		TypeHash hash = typeid(TModule).hash_code();

		// モジュール生成
		m_indices[hash] = m_modules.size();
		auto& rModule = m_modules.emplace_back(std::make_unique<TModule>(*this);
		//LOG_TRACE_EX("Engine", "モジュール追加 [{}]", name);

		return reinterpret_cast<TModule*>(rModule.get());

	}


	//@―---------------------------------------------------------------------------
	//! @brief  モジュールを取得
	//@―---------------------------------------------------------------------------
	template<class TModule>
	inline TModule* ModuleManager::get()const {

		// モジュール名取得
		TypeHash hash = typeid(TModule).hash_code();

		auto found = m_indices.find(hash);
		if (found != m_indices.end()) {
			return reinterpret_cast<TModule*>(m_modules[found->second].get());
		}
		return nullptr;

	}


    //! @endcond
}// namespcae ob