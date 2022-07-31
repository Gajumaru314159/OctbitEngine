//***********************************************************
//! @file
//! @brief		ボタン
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Button.h>
#include <Framework/Input/InputManager.h>

namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @berif	状態が state か
	//@―---------------------------------------------------------------------------
	bool Button::is(ButtonState state)const {
		if (auto device = InputManager::Instance().findDevice(m_deviceId, m_user)) {
			return device->getButtonStates(m_code)[state];
		}
		return false;
	}

	//@―---------------------------------------------------------------------------
	//! @berif	押されているか
	//@―---------------------------------------------------------------------------
	bool Button::down()const {
		return is(ButtonState::Down);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	離されているか
	//@―---------------------------------------------------------------------------
	bool Button::up()const {
		return is(ButtonState::Up);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	押された瞬間か
	//@―---------------------------------------------------------------------------
	bool Button::pressed()const {
		return is(ButtonState::Pressed);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	離された瞬間か
	//@―---------------------------------------------------------------------------
	bool Button::released()const {
		return is(ButtonState::Released);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	バインド
	//@―---------------------------------------------------------------------------
	bool Button::bindDown(ButtonHandle& handle, const ButtonDelegate& func)const {
		return bind(ButtonState::Down, handle, func);
	}
	bool Button::bindUp(ButtonHandle& handle, const ButtonDelegate& func)const {
		return bind(ButtonState::Up, handle, func);
	}
	bool Button::bindPressed(ButtonHandle& handle, const ButtonDelegate& func)const {
		return bind(ButtonState::Pressed, handle, func);
	}
	bool Button::bindReleased(ButtonHandle& handle, const ButtonDelegate& func)const {
		return bind(ButtonState::Released, handle, func);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	バインド
	//@―---------------------------------------------------------------------------
	bool Button::bind(ButtonState state, ButtonHandle& handle, const ButtonDelegate& func)const {
		if (auto device = InputManager::Instance().findDevice(m_deviceId, m_user)) {
			return device->bindButton(m_code, state, handle, func);
		}
		return false;
	}

}// namespace ob