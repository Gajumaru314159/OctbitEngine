//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Core.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Engine/IModule.h>
#include <Framework/Core/Misc/DateTime.h>
#include <Windows.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	Engine::Engine() {
		Logger::Instance();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	Engine::~Engine() {

		// モジュール開放
		while (!m_modules.empty()) {
			m_modules.back().reset();
			m_modules.pop_back();
		}

		LOG_INFO("[Shutdown OctbitEngine]");
	}


	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	bool Engine::update() {
		for (auto& rModule : m_modules) {
			rModule->update();
		}
		return m_terminate;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  停止要求
	//@―---------------------------------------------------------------------------
	void Engine::terminate() {
		m_terminate = true;
	}

}// namespace ob