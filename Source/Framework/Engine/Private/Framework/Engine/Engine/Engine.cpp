//***********************************************************
//! @file
//! @brief		エンジン
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Core.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Engine/IModule.h>

#include <Framework/Engine/Entity/EntityManager.h>
#include <Framework/Engine/Name/NameDictionary.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	Engine::Engine(EngineConfig&& config)
		: m_config(config)
	{
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	Engine::~Engine() {

		LOG_INFO("[Shutdown OctbitEngine]");
	}

	//@―---------------------------------------------------------------------------
	//! @brief				起動
	//@―---------------------------------------------------------------------------
	void Engine::startup() {
		EntityManager::Get();
		NameDictionary::Get();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	bool Engine::update() {

		visit([](engine::IModule& m) {m.update(); });
		return true;
	}

}// namespace ob

// グローバルオブジェクト
ob::engine::Engine* GEngine=nullptr;