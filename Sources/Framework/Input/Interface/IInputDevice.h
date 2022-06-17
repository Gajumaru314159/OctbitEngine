//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Duration.h>
#include <Framework/Input/Interface/IInputHandler.h>

namespace ob::input {

	class IInputDevice {
	public:

		virtual void update(Duration deltaTime) = 0;
		virtual void sendEvents(IInputHandler& handler) = 0;
	};

}// namespcae ob