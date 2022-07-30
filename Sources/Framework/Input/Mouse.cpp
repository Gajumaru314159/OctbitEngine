//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Mouse.h>
#include <Framework/Input/InputManager.h>

namespace ob::input {

	Vec2 Mouse::GetPos() {
		if (auto device = InputManager::Instance().findDevice(Code, 0)) {
			return {
				device->getAxisValue(enum_cast(MouseAxis::X)),
				device->getAxisValue(enum_cast(MouseAxis::Y))
			};
		}
		return { 0,0 };
	}

	Vec2 Mouse::GetDeltaPos() {
		if (auto device = InputManager::Instance().findDevice(Code, 0)) {
			return {
				device->getAxisValue(enum_cast(MouseAxis::DeltaX)),
				device->getAxisValue(enum_cast(MouseAxis::DeltaY))
			};
		}
		return { 0,0 };
	}

}// namespace ob