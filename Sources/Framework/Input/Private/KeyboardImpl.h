//***********************************************************
//! @file
//! @brief		キーボード・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Duration.h>
#include <Framework/Input/InputType.h>

#include <Framework/Core/Platform/WindowsHeaders.h>

namespace ob::input {

	struct KeyState {
		Duration duration;
		Key key;
		bool down;
		bool up;
	};



	class KeyboardImpl:Noncopyable {
	public:

		KeyboardImpl();

		void update();

		bool down(Key key)const;

		void bindButton(TriggerType,Key, ButtonHandle&, const ButtonDelegate&);

	private:
		HWND m_hWnd = nullptr;
		StaticArray<KeyState, 256> m_state;
		StaticArray<StaticArray<ButtonNotifier,enum_cast(TriggerType::MAX)>,enum_cast(Key::MAX)> m_notifiers;
	};


}// namespcae ob::input


namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @brief  キー押下
	//@―---------------------------------------------------------------------------
	inline bool KeyboardImpl::down(Key key)const {
		return m_state[enum_cast(key)].down;
	}

}