//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Private/MouseImpl.h>

namespace ob::input
{
	MouseImpl::MouseImpl()
		: m_position{}
	{
	}


	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	void MouseImpl::update() {
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
	Vec2 MouseImpl::position() {
		return m_deltaPos;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	bool MouseImpl::down(MouseButton) {
		return {};
	}

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	void MouseImpl::bindButton(TriggerType type, MouseButton button, ButtonHandle& handle, const ButtonDelegate& func) {
		auto index = enum_cast(button);
		if (is_in_range(index, m_notifiers)) {
			m_notifiers[index].add(handle, func);
		} else {
			handle.remove();
		}
	}

}// namespace ob