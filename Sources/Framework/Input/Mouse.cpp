//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Mouse.h>
#include <Framework/Input/InputManager.h>
#include <Framework/Input/Private/MouseImpl.h>

namespace ob::input {

	Vec2 MouseInput::value()const {
		return InputManager::Instance().m_mouse->position();
	}

	//bool MouseInput::down()const {
	//	return false;
	//}
	//bool MouseInput::up()const {
	//
	//}
	//bool MouseInput::pressed()const {
	//
	//}
	//Duration MouseInput::pressedSpan()const {
	//
	//}
	//Duration MouseInput::releasedSpan()const {
	//
	//}

	//void MouseInput::bindButton(MouseButton button,Handle& handle, const Notifier::delegate_type& func) {
	//
	//}

}// namespace ob