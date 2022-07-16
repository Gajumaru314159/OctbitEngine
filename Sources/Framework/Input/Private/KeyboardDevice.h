//***********************************************************
//! @file
//! @brief		キーボード・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Input.h>
#include <Framework/Core/Platform/WindowsHeaders.h>

namespace ob::input {

	struct KeyState {
		InputNotifier notifier;
		InputStates states;
		InputStates prevStates;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  キーボードデバイス
	//@―---------------------------------------------------------------------------
	class KeyboardDevice:public IInputDevice {
	public:

		KeyboardDevice();

		u32 getDeviceId()const override { return OB_FNV32("Keyboard"); };

		void update();

		InputStates getInputStates(u32 code)const;
		bool bind(u32 code, InputHandle& handle, const InputDelegate& func) override;

	private:
		HWND m_hWnd = nullptr;
		StaticArray<KeyState, enum_cast(Key::MAX)> m_state;
	};


}// namespcae ob::input