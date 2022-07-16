//***********************************************************
//! @file
//! @brief		マウスデバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Private/MouseDevice.h>
#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Window/WindowNativeAccessor.h>

namespace ob::input
{
	MouseDevice::MouseDevice()
		: m_position{}
		, m_interface(nullptr)
		, m_mouse(nullptr)
	{
		auto& window = platform::Window::getMainWindow();
		HWND hWnd = (HWND)platform::WindowNativeAccessor::getHWND(window);

		HRESULT result;
		result = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_interface, NULL);
		if (FAILED(result)) {
			LOG_ERROR("DirectInput8Createに失敗");
			return;
		}

		result = m_interface->CreateDevice(GUID_SysMouse, &m_mouse, NULL);	
		if (FAILED(result)) {
			LOG_ERROR("マウスの初期化に失敗");
			return;
		}

		// データフォーマットを設定
		result = m_mouse->SetDataFormat(&c_dfDIMouse2);	// マウス用のデータ・フォーマットを設定
		if (FAILED(result)) {
			LOG_ERROR("データフォーマットに失敗");
			return;
		}

		// モードを設定（フォアグラウンド＆非排他モード）
		result = m_mouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(result)) {
			LOG_ERROR("モードの設定に失敗");
			return;
		}

		// デバイスの設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

		result = m_mouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(result)) {
			LOG_ERROR("デバイスの設定に失敗");
			return;
		}

		// 入力制御開始
		m_mouse->Acquire();

	}

	MouseDevice::~MouseDevice() {
		if (m_mouse) {
			m_mouse->Release();
			m_mouse = nullptr;
		}
		if (m_interface) {
			m_interface->Release();
			m_interface = nullptr;
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	void MouseDevice::update() {
		// マウス座標取得
		if (POINT point; ::GetCursorPos(&point)) {
			Vec2 newPos(point.x,point.y);
			m_deltaPos = newPos - m_position;
			m_position = newPos;
		}

		m_state.rgbButtons[0] = 0;
		m_state.rgbButtons[1] = 0;
		m_state.rgbButtons[2] = 0;

		// 値の更新
		if (m_mouse) {
			if (FAILED(m_mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &m_state))) {
				m_mouse->Acquire(); // １発目や２発目にエラーが出るが無視してよい。
			}
		}


		auto set = [this](MouseButton key, int id) {
			u8 flag = m_state.rgbButtons[id];
			auto& state = m_states[enum_cast(key)];
			state.prevStates = state.states;
			state.states.clear();
			state.states.set(InputState::Pressed, flag & 0x80);
			state.states.set(InputState::Released, !state.states[InputState::Pressed]);
			state.states.set(InputState::Down, state.prevStates[InputState::Released] && state.states[InputState::Pressed]);
			state.states.set(InputState::Up, state.prevStates[InputState::Pressed] && state.states[InputState::Released]);
		};

		set(MouseButton::Left, 0);
		set(MouseButton::Right, 1);
		set(MouseButton::Middle, 2);

		if (m_states[enum_cast(MouseButton::Left)].states[InputState::Down]) {
			LOG_INFO("左クリック");
		}
		if (m_states[enum_cast(MouseButton::Left)].states[InputState::Up]) {
			LOG_INFO("左クリックEnd");
		};

		if (m_states[enum_cast(MouseButton::Right)].states[InputState::Down]) {
			LOG_INFO("右クリック");
		}
		if (m_states[enum_cast(MouseButton::Right)].states[InputState::Up]) {
			LOG_INFO("右クリックEnd");
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	Vec2 MouseDevice::position() {
		return m_deltaPos;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	bool MouseDevice::down(MouseButton) {
		return {};
	}

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	//void MouseDevice::bindButton(TriggerType type, MouseButton button, ButtonHandle& handle, const ButtonDelegate& func) {
	//	auto index = enum_cast(button);
	//	if (is_in_range(index, m_notifiers)) {
	//		m_notifiers[index].add(handle, func);
	//	} else {
	//		handle.remove();
	//	}
	//}

}// namespace ob