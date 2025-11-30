#include <Framework/Input/Device/KeyboardDevice.h>

namespace ob::input {
#if defined(OS_LINUX)

	namespace {
		struct KeyEntry {
			Key key;
			KeySym sym;
		};

		constexpr KeyEntry kKeyEntries[] = {
			{Key::F1, XK_F1},
			{Key::F2, XK_F2},
			{Key::F3, XK_F3},
			{Key::F4, XK_F4},
			{Key::F5, XK_F5},
			{Key::F6, XK_F6},
			{Key::F7, XK_F7},
			{Key::F8, XK_F8},
			{Key::F9, XK_F9},
			{Key::F10, XK_F10},
			{Key::F11, XK_F11},
			{Key::F12, XK_F12},
			{Key::Keypad0, XK_KP_0},
			{Key::Keypad1, XK_KP_1},
			{Key::Keypad2, XK_KP_2},
			{Key::Keypad3, XK_KP_3},
			{Key::Keypad4, XK_KP_4},
			{Key::Keypad5, XK_KP_5},
			{Key::Keypad6, XK_KP_6},
			{Key::Keypad7, XK_KP_7},
			{Key::Keypad8, XK_KP_8},
			{Key::Keypad9, XK_KP_9},
			{Key::KeypadPeriod, XK_KP_Decimal},
			{Key::KeypadDivide, XK_KP_Divide},
			{Key::KeypadMultiply, XK_KP_Multiply},
			{Key::KeypadSubstract, XK_KP_Subtract},
			{Key::KeypadAdd, XK_KP_Add},
			{Key::KeypadEnter, XK_KP_Enter},
			{Key::Alpha0, XK_0},
			{Key::Alpha1, XK_1},
			{Key::Alpha2, XK_2},
			{Key::Alpha3, XK_3},
			{Key::Alpha4, XK_4},
			{Key::Alpha5, XK_5},
			{Key::Alpha6, XK_6},
			{Key::Alpha7, XK_7},
			{Key::Alpha8, XK_8},
			{Key::Alpha9, XK_9},
			{Key::A, XK_A},
			{Key::B, XK_B},
			{Key::C, XK_C},
			{Key::D, XK_D},
			{Key::E, XK_E},
			{Key::F, XK_F},
			{Key::G, XK_G},
			{Key::H, XK_H},
			{Key::I, XK_I},
			{Key::J, XK_J},
			{Key::K, XK_K},
			{Key::L, XK_L},
			{Key::M, XK_M},
			{Key::N, XK_N},
			{Key::O, XK_O},
			{Key::P, XK_P},
			{Key::Q, XK_Q},
			{Key::R, XK_R},
			{Key::S, XK_S},
			{Key::T, XK_T},
			{Key::U, XK_U},
			{Key::V, XK_V},
			{Key::W, XK_W},
			{Key::X, XK_X},
			{Key::Y, XK_Y},
			{Key::Z, XK_Z},
			{Key::Exclaim, XK_exclam},
			{Key::DoubleQuote, XK_quotedbl},
			{Key::Hash, XK_numbersign},
			{Key::Dollar, XK_dollar},
			{Key::Percent, XK_percent},
			{Key::Ampersand, XK_ampersand},
			{Key::Quote, XK_apostrophe},
			{Key::LeftParen, XK_parenleft},
			{Key::RightParen, XK_parenright},
			{Key::Asterisk, XK_asterisk},
			{Key::Plus, XK_plus},
			{Key::Comma, XK_comma},
			{Key::Minus, XK_minus},
			{Key::Period, XK_period},
			{Key::Slash, XK_slash},
			{Key::Colon, XK_colon},
			{Key::Semicolon, XK_semicolon},
			{Key::Less, XK_less},
			{Key::Equals, XK_equal},
			{Key::Greater, XK_greater},
			{Key::Question, XK_question},
			{Key::At, XK_at},
			{Key::LeftBracket, XK_bracketleft},
			{Key::Backslash, XK_backslash},
			{Key::RightBracket, XK_bracketright},
			{Key::Caret, XK_asciicircum},
			{Key::Underscore, XK_underscore},
			{Key::BackQuote, XK_grave},
			{Key::LeftCurlyBracket, XK_braceleft},
			{Key::Pipe, XK_bar},
			{Key::RightCurlyBracket, XK_braceright},
			{Key::Tilde, XK_asciitilde},
			{Key::UpArrow, XK_Up},
			{Key::DownArrow, XK_Down},
			{Key::RightArrow, XK_Right},
			{Key::LeftArrow, XK_Left},
			{Key::Escape, XK_Escape},
			{Key::Space, XK_space},
			{Key::Tab, XK_Tab},
			{Key::Backspace, XK_BackSpace},
			{Key::Delete, XK_Delete},
			{Key::Enter, XK_Return},
			{Key::Insert, XK_Insert},
			{Key::CapsLock, XK_Caps_Lock},
			{Key::PrintScreen, XK_Print},
			{Key::ScrollLock, XK_Scroll_Lock},
			{Key::Pause, XK_Pause},
			{Key::Home, XK_Home},
			{Key::End, XK_End},
			{Key::PageUp, XK_Page_Up},
			{Key::PageDown, XK_Page_Down},
			{Key::NumLock, XK_Num_Lock},
			{Key::Shift, XK_Shift_L},
			{Key::LeftShift, XK_Shift_L},
			{Key::RightShift, XK_Shift_R},
			{Key::Ctrl, XK_Control_L},
			{Key::LeftCtrl, XK_Control_L},
			{Key::RightCtrl, XK_Control_R},
			{Key::Alt, XK_Alt_L},
			{Key::LeftAlt, XK_Alt_L},
			{Key::RightAlt, XK_Alt_R},
			{Key::Command, XK_Super_L},
			{Key::LeftCommand, XK_Super_L},
			{Key::RightCommand, XK_Super_R},
			{Key::Apple, XK_Super_L},
			{Key::LeftApple, XK_Super_L},
			{Key::RightApple, XK_Super_R},
			{Key::Windows, XK_Super_L},
			{Key::LeftWindows, XK_Super_L},
			{Key::RightWindows, XK_Super_R},
		};

		bool IsKeyPressed(::Display* display, const char* keymap, KeySym sym) {
			if (sym == NoSymbol || display == nullptr) return false;
			KeyCode code = XKeysymToKeycode(display, sym);
			if (code == 0) return false;
			return keymap[code / 8] & (1 << (code % 8));
		}
	} // namespace

	//! @brief  コンストラクタ
	KeyboardDevice::KeyboardDevice() {
		m_display = XOpenDisplay(nullptr);
		if (!m_display) {
			LOG_ERROR("XOpenDisplay に失敗しました。DISPLAY を確認してください。");
			return;
		}
		m_ownDisplay = true;
	}

	//! @brief  デストラクタ
	KeyboardDevice::~KeyboardDevice() {
		if (m_ownDisplay && m_display) {
			XCloseDisplay(m_display);
			m_display = nullptr;
		}
	}

	//! @brief	更新
	void KeyboardDevice::update() {
		if (!m_display) return;

		char keymap[32] = {};
		XQueryKeymap(m_display, keymap);

		auto set = [this, &keymap](Key key, KeySym sym) {
			auto& state = m_states[key];
			state.prev = state.next;
			state.next.clear();
			bool pressed = IsKeyPressed(m_display, keymap, sym);
			state.next.set(ButtonState::Pressed, pressed);
			state.next.set(ButtonState::Released, !pressed);
			state.next.set(ButtonState::Down, state.prev[ButtonState::Released] && pressed);
			state.next.set(ButtonState::Up, state.prev[ButtonState::Pressed] && !pressed);
		};

		for (const auto& entry : kKeyEntries) {
			set(entry.key, entry.sym);
		}

		for (auto& [key, state] : m_states) {
			const auto caller = [](KeyState& state, ButtonState buttonState) {
				if (state.prev[buttonState])state.notifiers[buttonState].invoke();
			};
			caller(state, ButtonState::Down);
			caller(state, ButtonState::Up);
			caller(state, ButtonState::Pressed);
			caller(state, ButtonState::Released);
		}
	}

#endif

}
