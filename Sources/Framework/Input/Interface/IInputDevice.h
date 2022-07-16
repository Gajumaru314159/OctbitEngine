//***********************************************************
//! @file
//! @brief		入力デバイス・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/InputType.h>

namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @brief  入力デバイス・インターフェイス
	//@―---------------------------------------------------------------------------
	class IInputDevice {
	public:

		virtual u32 getDeviceId()const = 0;
		virtual void update() {}

		virtual InputStates getInputStates(u32 code)const { return {}; }
		virtual f32  getF32(u32 code) const { return 0.0f; }
		virtual Vec2 getVec2(u32 code) const { return { 0.0f,0.0f }; }
		virtual Vec3 getVec3(u32 code) const { return { 0.0f,0.0f,0.0f }; }

		//@―---------------------------------------------------------------------------
		//! @berif	ボタンをバインド
		//@―---------------------------------------------------------------------------
		virtual bool bind(u32 code, InputHandle& handle, const InputDelegate& func) { return false; }

	};

}// namespcae ob