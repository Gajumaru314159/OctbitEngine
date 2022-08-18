//***********************************************************
//! @file
//! @brief		軸入力
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Axis.h>
#include <Framework/Input/InputManager.h>

namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @berif	値
	//@―---------------------------------------------------------------------------
	f32 Axis::value()const {
		if (auto device = InputManager::Instance().findDevice(m_deviceId, m_user)) {
			return device->getAxisValue(m_code);
		}
		return false;
	}

	//@―---------------------------------------------------------------------------
	//! @berif	バインド
	//@―---------------------------------------------------------------------------
	bool Axis::bind(AxisHandle& handle, const AxisDelegate& func)const {
		if (auto device = InputManager::Instance().findDevice(m_deviceId, m_user)) {
			return device->bindAxis( m_code, handle, func);
		}
		return false;
	}

}// namespace ob