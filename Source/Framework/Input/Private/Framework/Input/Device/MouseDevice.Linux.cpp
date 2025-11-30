#include <Framework/Input/Device/MouseDevice.h>

namespace ob::input {
#if defined(OS_LINUX)

	namespace {
		struct LinuxWindowHandle {
			::Display* display;
			::Window window;
		};
	}

	MouseDevice::MouseDevice(platform::Window& window)
		: m_display(nullptr)
		, m_window(0)
	{
		if (auto native = reinterpret_cast<LinuxWindowHandle*>(window.getHandle())) {
			m_display = native->display;
			m_window = native->window;
		}

		if (!m_display) {
			m_display = XOpenDisplay(nullptr);
			if (!m_display) {
				LOG_ERROR("XOpenDisplay に失敗しました。DISPLAY を確認してください。");
				return;
			}
			m_ownDisplay = true;
		}

		if (m_window == 0) {
			m_window = DefaultRootWindow(m_display);
		}
	}

	MouseDevice::~MouseDevice() {
		if (m_ownDisplay && m_display) {
			XCloseDisplay(m_display);
			m_display = nullptr;
		}
	}

	//! @brief  更新
	void MouseDevice::update() {
		if (!m_display) return;

		::Window root_return = 0;
		::Window child_return = 0;
		int root_x = 0;
		int root_y = 0;
		int win_x = 0;
		int win_y = 0;
		unsigned int mask_return = 0;

		if (!XQueryPointer(m_display, m_window, &root_return, &child_return, &root_x, &root_y, &win_x, &win_y, &mask_return)) {
			return;
		}

		Vec2 pos(static_cast<f32>(root_x), static_cast<f32>(root_y));
		Vec2 deltaPos = pos - m_position;
		m_position = pos;

		auto setButton = [this, mask_return](MouseButton key, unsigned int mask) {
			auto& state = m_states[key];
			state.prev = state.next;
			state.next.clear();
			bool pressed = mask != 0 && (mask_return & mask);
			state.next.set(ButtonState::Pressed, pressed);
			state.next.set(ButtonState::Released, !pressed);
			state.next.set(ButtonState::Down, state.prev[ButtonState::Released] && state.next[ButtonState::Pressed]);
			state.next.set(ButtonState::Up, state.prev[ButtonState::Pressed] && state.next[ButtonState::Released]);
		};

		setButton(MouseButton::Left, Button1Mask);
		setButton(MouseButton::Right, Button3Mask);
		setButton(MouseButton::Middle, Button2Mask);
		setButton(MouseButton::X1, 0);
		setButton(MouseButton::X2, 0);

		for (auto& [key, state] : m_axisStates) {
			state.prev = state.next;
		}
		m_axisStates[MouseAxis::X].next = pos.x;
		m_axisStates[MouseAxis::Y].next = pos.y;
		m_axisStates[MouseAxis::DeltaX].next = deltaPos.x;
		m_axisStates[MouseAxis::DeltaY].next = deltaPos.y;
		m_axisStates[MouseAxis::Wheel].next = 0.0f;

		for (auto& [key, state] : m_states) {
			const auto caller = [](KeyState& state, ButtonState buttonState) {
				if (state.prev[buttonState])state.notifiers[buttonState].invoke();
			};
			caller(state, ButtonState::Down);
			caller(state, ButtonState::Up);
			caller(state, ButtonState::Pressed);
			caller(state, ButtonState::Released);
		}

		for (auto& [key, state] : m_axisStates) {
			if (state.prev != state.next) state.notifier.invoke(state.next);
		}
	}

#endif

}
