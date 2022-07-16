//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Input.h>
#include <Framework/Input/InputManager.h>

namespace ob::input
{
	//@―---------------------------------------------------------------------------
	//! @berif	状態が state か
	//@―---------------------------------------------------------------------------
	bool Input::is(InputState state)const {
		if (auto device = InputManager::Instance().findDevice(m_deviceId, m_user)) {
			return device->getInputStates(m_code)[state];
		}
		return false;
	}

	//@―---------------------------------------------------------------------------
	//! @berif	押されているか
	//@―---------------------------------------------------------------------------
	bool Input::down()const {
		return is(InputState::Down);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	離されているか
	//@―---------------------------------------------------------------------------
	bool Input::up()const {
		return is(InputState::Up);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	押された瞬間か
	//@―---------------------------------------------------------------------------
	bool Input::pressed()const {
		return is(InputState::Pressed);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	離された瞬間か
	//@―---------------------------------------------------------------------------
	bool Input::released()const {
		return is(InputState::Released);
	}
	/*
	//@―---------------------------------------------------------------------------
	//! @berif	InputStateをバインド
	//@―---------------------------------------------------------------------------
	bool Input::bind(InputState state,InputHandle& handle, Delegate<void()> func) {
		if (func.empty()) { handle.remove(); return false; }

		return bindImpl(handle,
			[func](const detail::InputData& data) {
				if (std::holds_alternative<InputState>(data)) {
					if (std::get<InputState>(data)== InputState::Down) {
						func();
					}
				}
			}
		);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	f32をバインド
	//@―---------------------------------------------------------------------------
	bool Input::bind(InputHandle& handle, Delegate<void(f32)> func) {
		if (func.empty()) { handle.remove(); return false; }

		return bindImpl(handle,
			[func](const detail::InputData& data) {
				if (std::holds_alternative<f32>(data)) {
					func(std::get<f32>(data));
				}
			}
		);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	Vec2をバインド
	//@―---------------------------------------------------------------------------
	bool Input::bind(InputHandle& handle, Delegate<void(Vec2)> func) {
		if (func.empty()) { handle.remove(); return false; }

		return bindImpl(handle,
			[func](const detail::InputData& data) {
				if (std::holds_alternative<Vec2>(data)) {
					func(std::get<Vec2>(data));
				}
			}
		);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	Vec3をバインド
	//@―---------------------------------------------------------------------------
	bool Input::bind(InputHandle& handle, Delegate<void(Vec3)> func) {
		if (func.empty()) { handle.remove(); return false; }

		return bindImpl(handle,
			[func](const detail::InputData& data) {
				if (std::holds_alternative<Vec3>(data)) {
					func(std::get<Vec3>(data));
				}
			}
		);
	}*/

	//@―---------------------------------------------------------------------------
	//! @berif	入力をバインド
	//@―---------------------------------------------------------------------------
	bool Input::bindImpl(InputHandle& handle, const InputDelegate& func)const {
		return false;
		//if (m_pDevice==nullptr)return false;
		//return m_pDevice->bind(m_code,handle, func);
	}


}// namespace ob