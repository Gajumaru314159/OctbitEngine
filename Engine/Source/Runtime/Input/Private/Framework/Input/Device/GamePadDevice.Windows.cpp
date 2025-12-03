#include <Framework/Input/Device/GamePadDevice.h>
#include <Framework/Platform/Window.h>

namespace ob::input {
#if defined(OS_WINDOWS)

	BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) {
		auto* ptr = (Vector<DIDEVICEINSTANCE>*)pvRef;
		DIDEVICEINSTANCE tmp = *lpddi;
		ptr->push_back(tmp);
		return DIENUM_CONTINUE;
	}

	BOOL WINAPI DIEnumDevObjCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef) {
		auto* tmp = (Vector<DIDEVICEOBJECTINSTANCE>*)pvRef;
		tmp->push_back(*lpddoi);
		return DIENUM_CONTINUE;
	}

	GamePadDevice::GamePadDevice()
		: m_position{}
	{
		const auto& window = platform::Window::Main();
		HWND hWnd = (HWND)window.getHandle();

		HRESULT result;
		result = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_interface, NULL);
		if (FAILED(result)) {
			LOG_ERROR("DirectInput8Createに失敗");
			return;
		}

		Vector<DIDEVICEINSTANCE> instances;
		result = m_interface->EnumDevices(DI8DEVTYPE_JOYSTICK, DeviceFindCallBack, &instances, DIEDFL_ATTACHEDONLY);
		if (FAILED(result)) {
			LOG_ERROR("EnumDevicesに失敗");
			return;
		}

		if (instances.empty()) {
			LOG_ERROR("JoyStickが見つかりませんでした。");
			return;
		}

		result = m_interface->CreateDevice(instances.front().guidInstance, &m_device, NULL);
		if (FAILED(result)) {
			LOG_ERROR("マウスの初期化に失敗");
			return;
		}

		result = m_device->SetDataFormat(&c_dfDIJoystick);	// マウス用のデータ・フォーマットを設定
		if (FAILED(result)) {
			LOG_ERROR("データフォーマットに失敗");
			return;
		}

		result = m_device->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(result)) {
			LOG_ERROR("モードの設定に失敗");
			return;
		}

		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(DIPROPDWORD);
		diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;

		result = m_device->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if (FAILED(result)) {
			LOG_ERROR("デバイスの設定に失敗");
			return;
		}

		Vector<DIDEVICEOBJECTINSTANCE> axies;
		result = m_device->EnumObjects(DIEnumDevObjCallback, &axies, DIDFT_ALL);
		if (FAILED(result)) {
			LOG_ERROR("EnumObjectsに失敗");
			return;
		}

		for (auto& axis : axies) {
			DIPROPRANGE diproprg;
			diproprg.diph.dwSize = sizeof(DIPROPRANGE);
			diproprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			diproprg.diph.dwHow = DIPH_BYID;
			diproprg.diph.dwObj = axis.dwType;
			diproprg.lMin = -255;
			diproprg.lMax = 255;
			m_device->SetProperty(DIPROP_RANGE, &diproprg.diph);
		}

		m_device->Poll();
		m_device->Acquire();
	}

	GamePadDevice::~GamePadDevice() {
		if (m_device) {
			m_device->Release();
			m_device = nullptr;
		}
		if (m_interface) {
			m_interface->Release();
			m_interface = nullptr;
		}
	}

	//! @brief  更新
	void GamePadDevice::update() {
		m_state = {};
		if (m_device) {
			if (FAILED(m_device->GetDeviceState(sizeof(DIJOYSTATE), &m_state))) {
				m_device->Acquire();
			}
		}

		if (m_state.rgbButtons[0] & 0x80) {
			LOG_INFO("ボタンが押されました。");
		}
	}

#endif

}
