﻿//***********************************************************
//! @file
//! @brief		キーボード・デバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Interface/IInputDevice.h>
#include <Framework/Input/Keyboard.h>
#include <Framework/Core/Platform/WindowsHeaders.h>

namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @brief  キーボード・デバイス
	//@―---------------------------------------------------------------------------
	class KeyboardDevice:public IInputDevice {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		KeyboardDevice();

		//@―---------------------------------------------------------------------------
		//! @brief  デストラクタ
		//@―---------------------------------------------------------------------------
		~KeyboardDevice();

		//@―---------------------------------------------------------------------------
		//! @brief  デバイスID
		//@―---------------------------------------------------------------------------
		DeviceID getDeviceId()const override { return Keyboard::ID; }

		//@―---------------------------------------------------------------------------
		//! @brief  更新
		//@―---------------------------------------------------------------------------
		void update();


		//@―---------------------------------------------------------------------------
		//! @brief  ボタンの入力状態を取得
		//@―---------------------------------------------------------------------------
		ButtonStates getButtonStates(u32 code)const override;

		//@―---------------------------------------------------------------------------
		//! @brief  ボタン入力イベントをバインド
		//@―---------------------------------------------------------------------------
		bool bindButton(u32 code, ButtonState state, ButtonHandle& handle, const ButtonDelegate& func) override;

	private:
		struct KeyState {
			HashMap<ButtonState,ButtonNotifier> notifiers;
			ButtonStates prev;
			ButtonStates next;
		};
	private:

		HWND m_hWnd = nullptr;
		HashMap<Key, KeyState> m_states;;

	};


}// namespcae ob::input