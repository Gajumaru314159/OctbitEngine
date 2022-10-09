﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Core.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Engine/IModule.h>
#include <Framework/Core/Misc/DateTime.h>

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

}// namespace ob

ob::engine::Engine* GEngine=nullptr;