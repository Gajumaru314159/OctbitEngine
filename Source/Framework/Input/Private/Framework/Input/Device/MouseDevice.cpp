//***********************************************************
//! @file
//! @brief		マウス・デバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Device/MouseDevice.h>
#include <Framework/Platform/Window.h>
#include <Framework/Platform/WindowNativeAccessor.h>

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


		// 値の更新
		m_mouseState={};
		if (m_mouse) {
			if (FAILED(m_mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &m_mouseState))) {
				m_mouse->Acquire(); // １発目や２発目にエラーが出るが無視してよい。
			}
		}

		// マウス座標取得
		Vec2 pos(0.0f, 0.0f);
		Vec2 deltaPos(0.0f,0.0f);
		if (POINT point; ::GetCursorPos(&point)) {
			pos.set((f32)point.x, (f32)point.y);
			deltaPos = pos - m_position;
		}
		m_position = pos;

		// ボタン更新
		auto set = [this](MouseButton key, int id) {
			u8 flag = m_mouseState.rgbButtons[id];
			auto& state = m_states[key];
			state.prev= state.next;
			state.next.clear();
			state.next.set(ButtonState::Pressed, flag & 0x80);
			state.next.set(ButtonState::Released, !(flag & 0x80));
			state.next.set(ButtonState::Down, state.prev[ButtonState::Released] && state.next[ButtonState::Pressed]);
			state.next.set(ButtonState::Up, state.prev[ButtonState::Pressed] && state.next[ButtonState::Released]);
		};

		set(MouseButton::Left, 0);
		set(MouseButton::Right, 1);
		set(MouseButton::Middle, 2);


		// 軸更新
		for (auto& [key, state] : m_axisStates) {
			state.prev = state.next;
		}
		m_axisStates[MouseAxis::X].next = pos.x;
		m_axisStates[MouseAxis::Y].next = pos.y;
		m_axisStates[MouseAxis::DeltaX].next = deltaPos.x;
		m_axisStates[MouseAxis::DeltaY].next = deltaPos.y;


		// バインドしているイベントを呼び出し
		for (auto& [key, state] : m_states) {
			const auto caller = [](KeyState& state, ButtonState buttonState) {
				if (state.prev[buttonState])state.notifiers[buttonState].invoke();
			};
			caller(state, ButtonState::Down);
			caller(state, ButtonState::Up);
			caller(state, ButtonState::Pressed);
			caller(state, ButtonState::Released);
		}

		// バインドしているイベントを呼び出し
		for (auto& [key, state] : m_axisStates) {
			if(state.prev!=state.next)state.notifier.invoke(state.next);
		}

	}


	//@―---------------------------------------------------------------------------
	//! @brief  ボタンの入力状態を取得
	//@―---------------------------------------------------------------------------
	ButtonStates MouseDevice::getButtonStates(u32 code)const {
		auto key = static_cast<MouseButton>(code);
		auto found = m_states.find(key);
		if (found == m_states.end())return {};
		return found->second.next;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  軸の入力状態を取得
	//@―---------------------------------------------------------------------------
	f32 MouseDevice::getAxisValue(u32 code)const {
		auto key = static_cast<MouseAxis>(code);
		auto found = m_axisStates.find(key);
		if (found == m_axisStates.end())return 0.0f;
		return found->second.next;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ボタン入力イベントをバインド
	//@―---------------------------------------------------------------------------
	bool MouseDevice::bindButton(u32 code, ButtonState state, ButtonHandle& handle, const ButtonDelegate& func) {
		auto key = static_cast<MouseButton>(code);
		m_states[key].notifiers[state].add(handle, func);
		return true;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ボタン入力イベントをバインド
	//@―---------------------------------------------------------------------------
	bool MouseDevice::bindAxis(u32 code, AxisHandle& handle, const AxisDelegate& func) {
		auto key = static_cast<MouseAxis>(code);
		m_axisStates[key].notifier.add(handle, func);
		return true;
	}

}// namespace ob