//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/String/Name.h>

namespace ob::engine {

	class IModule;

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class Engine {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------
		Engine();
		~Engine();

		bool update();
		void terminate();

	public:

		//@―---------------------------------------------------------------------------
		//! @brief  モジュールを追加
		//@―---------------------------------------------------------------------------
		template<class TModule, typename... Args>
		TModule* add(Args&& ...args) {

			auto name = TModule::Name();

			// 登録済みかチェック
			if (auto found = get<TModule>()) {
				return found;
			}

			m_indices[name] = m_modules.size();
			auto& rModule = m_modules.emplace_back(std::make_unique<TModule>(*this, std::forward(args)...));
			LOG_TRACE_EX("Engine", "モジュール追加 [{}]", name);

			return reinterpret_cast<TModule*>(rModule.get());

		}


		//@―---------------------------------------------------------------------------
		//! @brief  モジュールを取得
		//@―---------------------------------------------------------------------------
		template<class TModule>
		TModule* get()const {

			auto name = TModule::Name();

			auto found = m_indices.find(name);
			if (found != m_indices.end()) {
				return reinterpret_cast<TModule*>(m_modules[found->second].get());
			}
			return nullptr;

		}

	private:


		Array<UPtr<IModule>> m_modules;
		HashMap<Name, size_t> m_indices;
		Atomic<bool> m_terminate;

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond



	//! @endcond
}// namespcae ob