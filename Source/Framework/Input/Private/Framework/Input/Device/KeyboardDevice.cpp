//***********************************************************
//! @file
//! @brief		キーボード・デバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Device/KeyboardDevice.h>
#include <magic_enum.hpp>

namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	KeyboardDevice::KeyboardDevice() {
		m_hWnd = nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	KeyboardDevice::~KeyboardDevice() {

	}

	//@―---------------------------------------------------------------------------
	//! @brief	更新
	//@―---------------------------------------------------------------------------
	void KeyboardDevice::update() {

		u8 buf[256] = {};
		if (::GetKeyboardState(buf)) {

			auto set = [this, &buf](Key key, int id) {
				u8 flag = buf[id];
				auto& state = m_states[key];
				state.prev = state.next;
				state.next.clear();
				state.next.set(ButtonState::Pressed, flag & 0x80);
				state.next.set(ButtonState::Released, !(flag & 0x80));
				state.next.set(ButtonState::Down, state.prev[ButtonState::Released] && state.next[ButtonState::Pressed]);
				state.next.set(ButtonState::Up, state.prev[ButtonState::Pressed] && state.next[ButtonState::Released]);
			};

			set(Key::F1, VK_F1);
			set(Key::F2, VK_F2);					//!< F2
			set(Key::F3, VK_F3);					//!< F3
			set(Key::F4, VK_F4);					//!< F4
			set(Key::F5, VK_F5);					//!< F5
			set(Key::F6, VK_F6);					//!< F6
			set(Key::F7, VK_F7);					//!< F7
			set(Key::F8, VK_F8);					//!< F8
			set(Key::F9, VK_F9);					//!< F9
			set(Key::F10, VK_F10);					//!< F10
			set(Key::F11, VK_F11);					//!< F11
			set(Key::F12, VK_F12);					//!< F12
			set(Key::Keypad0, VK_NUMPAD0);			//!< テンキー0
			set(Key::Keypad1, VK_NUMPAD1);			//!< テンキー1
			set(Key::Keypad2, VK_NUMPAD2);			//!< テンキー2
			set(Key::Keypad3, VK_NUMPAD3);			//!< テンキー3
			set(Key::Keypad4, VK_NUMPAD4);			//!< テンキー4
			set(Key::Keypad5, VK_NUMPAD5);			//!< テンキー5
			set(Key::Keypad6, VK_NUMPAD6);			//!< テンキー6
			set(Key::Keypad7, VK_NUMPAD7);			//!< テンキー7
			set(Key::Keypad8, VK_NUMPAD8);			//!< テンキー8
			set(Key::Keypad9, VK_NUMPAD9);			//!< テンキー9
			set(Key::KeypadPeriod, VK_DECIMAL);		//!< テンキー.
			set(Key::KeypadDivide, VK_DIVIDE);		//!< テンキー/
			set(Key::KeypadMultiply, VK_MULTIPLY);	//!< テンキー*
			set(Key::KeypadSubstract, VK_SUBTRACT);		//!< テンキー-
			set(Key::KeypadAdd, VK_ADD);			//!< テンキー+
			set(Key::KeypadEnter, VK_RETURN);		//!< テンキーEnter
			set(Key::Alpha0, '0');				//!< 数字キー0
			set(Key::Alpha1, '1');				//!< 数字キー1
			set(Key::Alpha2, '2');				//!< 数字キー2
			set(Key::Alpha3, '3');				//!< 数字キー3
			set(Key::Alpha4, '4');				//!< 数字キー4
			set(Key::Alpha5, '5');				//!< 数字キー5
			set(Key::Alpha6, '6');				//!< 数字キー6
			set(Key::Alpha7, '7');				//!< 数字キー7
			set(Key::Alpha8, '8');				//!< 数字キー8
			set(Key::Alpha9, '9');				//!< 数字キー9
			set(Key::A, 'A');					//!< A
			set(Key::B, 'B');					//!< B
			set(Key::C, 'C');					//!< C
			set(Key::D, 'D');					//!< D
			set(Key::E, 'E');					//!< E
			set(Key::F, 'F');					//!< F
			set(Key::G, 'G');					//!< G
			set(Key::H, 'H');					//!< H
			set(Key::I, 'I');					//!< I
			set(Key::J, 'J');					//!< J
			set(Key::K, 'K');					//!< K
			set(Key::L, 'L');					//!< L
			set(Key::M, 'M');					//!< M
			set(Key::N, 'N');					//!< N
			set(Key::O, 'O');					//!< O
			set(Key::P, 'P');					//!< P
			set(Key::Q, 'Q');					//!< Q
			set(Key::R, 'R');					//!< R
			set(Key::S, 'S');					//!< S
			set(Key::T, 'T');					//!< T
			set(Key::U, 'U');					//!< U
			set(Key::V, 'V');					//!< V
			set(Key::W, 'W');					//!< W
			set(Key::X, 'X');					//!< X
			set(Key::Y, 'Y');					//!< Y
			set(Key::Z, 'Z');					//!< Z
			set(Key::Exclaim, '!');				//!< !
			set(Key::DoubleQuote, '\"');		//!< "
			set(Key::Hash, '#');				//!< #
			set(Key::Dollar, '$');				//!< $
			set(Key::Percent, '%');				//!< %
			set(Key::Ampersand, '&');			//!< &
			set(Key::Quote, '\'');				//!< '
			set(Key::LeftParen, '(');			//!< (
			set(Key::RightParen, ')');			//!< )
			set(Key::Asterisk, '*');			//!< *
			set(Key::Plus, '+');				//!< +
			set(Key::Comma, ',');				//!< , '');
			set(Key::Minus, '-');				//!< -
			set(Key::Period, '.');				//!< .
			set(Key::Slash, '/');				//!< /
			set(Key::Colon, ':');				//!< :
			set(Key::Semicolon, ';');			//!< ;
			set(Key::Less, '<');				//!< <
			set(Key::Equals, '=');				//!< =
			set(Key::Greater, '>');				//!< >
			set(Key::Question, '?');			//!< ?
			set(Key::At, '@');					//!< @
			set(Key::LeftBracket, '[');			//!< [
			set(Key::Backslash, '\\');			//!< \ または ＼
			set(Key::RightBracket, ']');		//!< ]
			set(Key::Caret, '^');				//!< ^
			set(Key::Underscore, '_');			//!< _
			set(Key::BackQuote, '`');			//!< `
			set(Key::LeftCurlyBracket, '{');	//!< {
			set(Key::Pipe, '|');				//!< |
			set(Key::RightCurlyBracket, '}');	//!< }
			set(Key::Tilde, '~');				//!< ~
			set(Key::UpArrow, VK_UP);			//!< ↑
			set(Key::DownArrow, VK_DOWN);		//!< ↓
			set(Key::RightArrow, VK_RIGHT);		//!< →
			set(Key::LeftArrow, VK_LEFT);		//!< ←
			set(Key::Escape, VK_ESCAPE);		//!< Escape
			set(Key::Space, VK_SPACE);			//!< Space
			set(Key::Tab, VK_TAB);				//!< Tab
			set(Key::Backspace, VK_BACK);		//!< Backspace
			set(Key::Delete, VK_DELETE);		//!< Delete
			set(Key::Enter, VK_RETURN);			//!< Enter / Return
			set(Key::Insert, VK_INSERT);		//!< Insert
			set(Key::CapsLock, VK_CAPITAL);		//!< CapsLock
			set(Key::PrintScreen, VK_SNAPSHOT);	//!< PrintScreen
			set(Key::ScrollLock, VK_SCROLL);	//!< ScrollLock
			set(Key::Pause, VK_PAUSE);			//!< Pause / Break
			set(Key::Home, VK_HOME);			//!< Home
			set(Key::End, VK_END);				//!< End
			//set(Key::PageUp, );				//!< PageUp
			//set(Key::PageDown, '');			//!< PageDown
			set(Key::NumLock, VK_NUMLOCK);		//!< NumLock
			//set(Key::Shift, );				//!< Shift
			set(Key::LeftShift, VK_LSHIFT);		//!< 左Shift
			set(Key::RightShift, VK_RSHIFT);	//!< 右Shift
			//set(Key::Ctrl, VK_);				//!< Ctrl
			set(Key::LeftCtrl, VK_LCONTROL);	//!< 左Ctrl
			set(Key::RightCtrl, VK_RCONTROL);	//!< 右Ctrl
			//set(Key::Alt, '');				//!< Alt
			set(Key::LeftAlt, VK_LMENU);		//!< 左Alt
			set(Key::RightAlt, VK_RMENU);		//!< 右Alt
			//set(Key::Command, VK_LCONTROL);			//!< Command
			//set(Key::LeftCommand, VK_LCONTROL);		//!< 左Command
			//set(Key::RightCommand, VK_RCONTROL);		//!< 右Command
			//set(Key::Apple, '');				//!< Appleキー
			//set(Key::LeftApple, '');			//!< 左Appleキー
			//set(Key::RightApple, '');			//!< 右Appleキー
			//set(Key::Windows, '');			//!< Windowsキー
			set(Key::LeftWindows, VK_LWIN);		//!< 左Windowsキー
			set(Key::RightWindows, VK_RWIN);	//!< 右Windowsキー

		}
		// TODO ウィンドウにフォーカスしているかで入力を切り替える

		// バインドしているイベントを呼び出し
		for (auto& [key,state]:m_states) {
			const auto caller = [](KeyState& state,ButtonState buttonState) {
				if (state.prev[buttonState])state.notifiers[buttonState].invoke();
			};
			caller(state,ButtonState::Down);
			caller(state,ButtonState::Up);
			caller(state,ButtonState::Pressed);
			caller(state,ButtonState::Released);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ボタンの入力状態を取得
	//@―---------------------------------------------------------------------------
	ButtonStates KeyboardDevice::getButtonStates(u32 code)const {
		auto key = static_cast<Key>(code);
		auto found = m_states.find(key);
		if (found == m_states.end())return {};
		return found->second.next;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ボタン入力イベントをバインド
	//@―---------------------------------------------------------------------------
	bool KeyboardDevice::bindButton(u32 code, ButtonState state, ButtonHandle& handle, const ButtonDelegate& func) {
		auto key = static_cast<Key>(code);
		m_states[key].notifiers[state].add(handle, func);
		return true;
	}

}// namespace ob