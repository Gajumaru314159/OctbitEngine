//***********************************************************
//! @file
//! @brief		マウス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Mouse.h>
#include <Framework/Input/InputManager.h>

namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @brief  マウスポインタの座標を取得
	//@―---------------------------------------------------------------------------
	Vec2 Mouse::GetPos() {
		if (auto device = InputManager::Instance().findDevice(ID, 0)) {
			return {
				device->getAxisValue(enum_cast(MouseAxis::X)),
				device->getAxisValue(enum_cast(MouseAxis::Y))
			};
		}
		return { 0,0 };
	}

	//@―---------------------------------------------------------------------------
	//! @brief  マウスポインタの移動量を取得
	//@―---------------------------------------------------------------------------
	Vec2 Mouse::GetDeltaPos() {
		if (auto device = InputManager::Instance().findDevice(ID, 0)) {
			return {
				device->getAxisValue(enum_cast(MouseAxis::DeltaX)),
				device->getAxisValue(enum_cast(MouseAxis::DeltaY))
			};
		}
		return { 0,0 };
	}

}// namespace ob