#include <Framework/Input/Device/KeyboardDevice.h>

namespace ob::input {

	//! @brief  ボタンの入力状態を取得
	ButtonStates KeyboardDevice::getButtonStates(u32 code) const {
		auto key = static_cast<Key>(code);
		auto found = m_states.find(key);
		if (found == m_states.end()) return {};
		return found->second.next;
	}

	//! @brief  ボタン入力イベントをバインド
	bool KeyboardDevice::bindButton(u32 code, ButtonState state, ButtonHandle& handle, const ButtonDelegate& func) {
		auto key = static_cast<Key>(code);
		m_states[key].notifiers[state].add(handle, func);
		return true;
	}

}
