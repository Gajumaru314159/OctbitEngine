//***********************************************************
//! @file
//! @brief		マウスデバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/DirectInputDevice.h>
#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Window/WindowNativeAccessor.h>

namespace ob::input
{
	BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi,LPVOID pvRef)
	{
		auto* ptr = (Array<DIDEVICEINSTANCE>*)pvRef;
		DIDEVICEINSTANCE tmp = *lpddi;

		// 配列に格納
		ptr->push_back(tmp);

		return DIENUM_CONTINUE;
	}

	// デバイスのオブジェクトを列挙するコールバック関数（static宣言）
	BOOL WINAPI DIEnumDevObjCallback(
		LPCDIDEVICEOBJECTINSTANCE lpddoi,
		LPVOID pvRef
	)
	{
		// 第2引数をvector<DIDEVICEOBJECTINSTANCE>へのポインタに型変換
		auto* tmp = (Array<DIDEVICEOBJECTINSTANCE>*)pvRef;

		// 配列にオブジェクトの情報を格納
		tmp->push_back(*lpddoi);

		// 列挙は全て終われば自動的に終了するので、
		// 列挙し続ける
		return DIENUM_CONTINUE;
	}


	DirectInputDevice::DirectInputDevice()
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

		// デバイスの列挙
		Array<DIDEVICEINSTANCE> instances;
		result = m_interface->EnumDevices(DI8DEVTYPE_JOYSTICK,DeviceFindCallBack,&instances,DIEDFL_ATTACHEDONLY);
		if (FAILED(result)) {
			LOG_ERROR("EnumDevicesに失敗");
			return;
		}

		if (instances.empty()) {
			LOG_ERROR("JoyStickが見つかりませんでした。");
			return;
		}

		result = m_interface->CreateDevice(instances.front().guidInstance, &m_mouse, NULL);
		if (FAILED(result)) {
			LOG_ERROR("マウスの初期化に失敗");
			return;
		}

		// データフォーマットを設定
		result = m_mouse->SetDataFormat(&c_dfDIJoystick);	// マウス用のデータ・フォーマットを設定
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
		diprop.diph.dwSize = sizeof(DIPROPDWORD); // 使用される構造体のサイズ
		diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER); // DIPROPHEADER構造体のサイズ
		diprop.diph.dwHow = DIPH_DEVICE; // 対象（ここではデバイス）
		diprop.diph.dwObj = 0; // デバイスを対象とする時はいつも0
		diprop.dwData = DIPROPAXISMODE_ABS; // 絶対値モードに設定

		result = m_mouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(result)) {
			LOG_ERROR("デバイスの設定に失敗");
			return;
		}

		Array<DIDEVICEOBJECTINSTANCE> axies;
		result = m_mouse->EnumObjects(DIEnumDevObjCallback,&axies,DIDFT_ALL);
		if (FAILED(result)) {
			LOG_ERROR("EnumObjectsに失敗");
			return;
		}

		// 軸の値の設定
		for (auto& axis: axies) {
			DIPROPRANGE diproprg;
			diproprg.diph.dwSize = sizeof(DIPROPRANGE); // 使用される構造体のサイズ
			diproprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); // DIPROPHEADER構造体のサイズ
			diproprg.diph.dwHow = DIPH_BYID; // 対象（ここでは軸）
			diproprg.diph.dwObj = axis.dwType; // 対象となる軸
			diproprg.lMin = -255; // 最小値
			diproprg.lMax = 255; // 最大値

			// 設定
			m_mouse->SetProperty(DIPROP_RANGE, &diproprg.diph);
		}

		// 入力制御開始
		m_mouse->Poll();
		m_mouse->Acquire();

	}

	DirectInputDevice::~DirectInputDevice() {
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
	void DirectInputDevice::update() {


		// 値の更新
		m_mouseState={};
		if (m_mouse) {
			if (FAILED(m_mouse->GetDeviceState(sizeof(DIJOYSTATE), &m_mouseState))) {
				m_mouse->Acquire(); // １発目や２発目にエラーが出るが無視してよい。
			}
		}

		if (m_mouseState.rgbButtons[0] & 0x80) {
			LOG_INFO("ボタンが押されました。");
		}

	}


	//@―---------------------------------------------------------------------------
	//! @brief  ボタンの入力状態を取得
	//@―---------------------------------------------------------------------------
	ButtonStates DirectInputDevice::getButtonStates(u32 code)const {
		auto key = static_cast<MouseButton>(code);
		auto found = m_states.find(key);
		if (found == m_states.end())return {};
		return found->second.next;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  軸の入力状態を取得
	//@―---------------------------------------------------------------------------
	f32 DirectInputDevice::getAxisValue(u32 code)const {
		auto key = static_cast<MouseAxis>(code);
		auto found = m_axisStates.find(key);
		if (found == m_axisStates.end())return 0.0f;
		return found->second.next;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ボタン入力イベントをバインド
	//@―---------------------------------------------------------------------------
	bool DirectInputDevice::bindButton(u32 code, ButtonState state, ButtonHandle& handle, const ButtonDelegate& func) {
		auto key = static_cast<MouseButton>(code);
		m_states[key].notifiers[state].add(handle, func);
		return true;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ボタン入力イベントをバインド
	//@―---------------------------------------------------------------------------
	bool DirectInputDevice::bindAxis(u32 code, AxisHandle& handle, const AxisDelegate& func) {
		auto key = static_cast<MouseAxis>(code);
		m_axisStates[key].notifier.add(handle, func);
		return true;
	}

}// namespace ob