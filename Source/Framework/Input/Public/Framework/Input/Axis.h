//***********************************************************
//! @file
//! @brief		軸入力
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/InputType.h>


//! @brief	ボタン定義マクロ
#define OB_DECL_AXIS(type,name) static constexpr Axis name{ ID,type::name };

namespace ob::input {

	//! @brief  軸
	class Axis {
	public:

		//! @brief	デフォルトコンストラクタ
		Axis() = default;

		//! @brief			コンストラクタ
		//! @param deviceID	デバイスタイプ
		//! @param code		コード
		//! @param user		ユーザ番号
		constexpr Axis(DeviceID deviceID, u32 code, u32 user = 0) :m_deviceId(deviceID), m_code(code), m_user(user) {}

		//! @brief	コンストラクタ
		template<typename T>
		constexpr Axis(DeviceID type, T code, u32 user = 0) : Axis(type, enum_cast(code), user) {}

	public:

		//! @brief	値
		f32  value() const;

	public:

		//! @brief	バインド
		bool bind(AxisHandle& handle, const AxisDelegate& func)const;

	private:
		DeviceID m_deviceId;
		u32 m_user;
		u32 m_code;
	};

}