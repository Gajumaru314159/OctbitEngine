//***********************************************************
//! @file
//! @brief		ボタン入力
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Button.h>
#include <Framework/Input/InputManager.h>

namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @brief	状態が state か
	//@―---------------------------------------------------------------------------
	bool Button::is(ButtonState state)const {
		if (auto m = InputModule::Get()) {
			if (auto device = m->findDevice(m_deviceId, m_user)) {
				return device->getButtonStates(m_code)[state];
			}
		}
		return false;

	}

	//@―---------------------------------------------------------------------------
	//! @brief	押された瞬間か
	//@―---------------------------------------------------------------------------
	bool Button::down()const {
		return is(ButtonState::Down);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	離された瞬間か
	//@―---------------------------------------------------------------------------
	bool Button::up()const {
		return is(ButtonState::Up);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	押されているか
	//@―---------------------------------------------------------------------------
	bool Button::pressed()const {
		return is(ButtonState::Pressed);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	離されているか
	//@―---------------------------------------------------------------------------
	bool Button::released()const {
		return is(ButtonState::Released);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	バインド
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
	//! @brief	バインド
	//@―---------------------------------------------------------------------------
	bool Button::bind(ButtonState state, ButtonHandle& handle, const ButtonDelegate& func)const {
		if (auto m = InputModule::Get()) {
			if (auto device = m->findDevice(m_deviceId, m_user)) {
				return device->bindButton(m_code, state, handle, func);
			}
		}
		return false;

	}

}