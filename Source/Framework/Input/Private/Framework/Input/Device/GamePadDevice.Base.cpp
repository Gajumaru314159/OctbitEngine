#include <Framework/Input/Device/GamePadDevice.h>

namespace ob::input {

	//! @brief  ボタンの入力状態を取得
	ButtonStates GamePadDevice::getButtonStates(u32 code) const {
		auto key = static_cast<MouseButton>(code);
		auto found = m_states.find(key);
		if (found == m_states.end()) return {};
		return found->second.next;
	}

	//! @brief  軸の入力状態を取得
	f32 GamePadDevice::getAxisValue(u32 code) const {
		auto key = static_cast<MouseAxis>(code);
		auto found = m_axisStates.find(key);
		if (found == m_axisStates.end()) return 0.0f;
		return found->second.next;
	}

	//! @brief  ボタン入力イベントをバインド
	bool GamePadDevice::bindButton(u32 code, ButtonState state, ButtonHandle& handle, const ButtonDelegate& func) {
		auto key = static_cast<MouseButton>(code);
		m_states[key].notifiers[state].add(handle, func);
		return true;
	}

	//! @brief  ボタン入力イベントをバインド
	bool GamePadDevice::bindAxis(u32 code, AxisHandle& handle, const AxisDelegate& func) {
		auto key = static_cast<MouseAxis>(code);
		m_axisStates[key].notifier.add(handle, func);
		return true;
	}

}
