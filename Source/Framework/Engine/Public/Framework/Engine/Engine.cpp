//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Core.h>
#include <Framework/Engine/Engine.h>



namespace ob {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	Engine::Engine() {
		// TODO DLL対応
		assert(m_instance!=nullptr);
		m_instance = nullptr;
	}

	Engine::~Engine() {
		LOG_INFO("[Shutdown OctbitEngine]");
	}

	bool Engine::IsActive()noexcept {
		return m_instance != nullptr;
	}


}// namespace ob