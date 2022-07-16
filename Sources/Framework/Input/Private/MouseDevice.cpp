//***********************************************************
//! @file
//! @brief		マウスデバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Private/MouseDevice.h>
#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Window/WindowNativeAccessor.h>

namespace ob::input
{
	MouseDevice::MouseDevice()
		: m_position{}
	{
		platform::Window::getMainWindow();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	void MouseDevice::update() {
		// マウス座標取得
		if (POINT point; ::GetCursorPos(&point)) {
			Vec2 newPos(point.x,point.y);
			m_deltaPos = newPos - m_position;
			m_position = newPos;
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	Vec2 MouseDevice::position() {
		return m_deltaPos;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	bool MouseDevice::down(MouseButton) {
		return {};
	}

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	//void MouseDevice::bindButton(TriggerType type, MouseButton button, ButtonHandle& handle, const ButtonDelegate& func) {
	//	auto index = enum_cast(button);
	//	if (is_in_range(index, m_notifiers)) {
	//		m_notifiers[index].add(handle, func);
	//	} else {
	//		handle.remove();
	//	}
	//}

}// namespace ob