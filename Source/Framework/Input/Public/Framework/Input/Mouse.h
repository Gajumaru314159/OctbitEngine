//***********************************************************
//! @file
//! @brief		マウス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Button.h>
#include <Framework/Input/Axis.h>

namespace ob::input {

	class Mouse {
		OB_DECL_INPUT_DEVICE_ID("Mouse");
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  マウスポインタの座標を取得
		//@―---------------------------------------------------------------------------
		static Vec2 GetPos();

		//@―---------------------------------------------------------------------------
		//! @brief  マウスポインタの移動量を取得
		//@―---------------------------------------------------------------------------
		static Vec2 GetDeltaPos();

	public:

		OB_DECL_BUTTON(MouseButton,Left);	//!< 左ボタン
		OB_DECL_BUTTON(MouseButton,Right);	//!< 右ボタン
		OB_DECL_BUTTON(MouseButton,Middle);	//!< 中ボタン
		OB_DECL_BUTTON(MouseButton,X1);		//!< 拡張1ボタン
		OB_DECL_BUTTON(MouseButton,X2);		//!< 拡張21ボタン
		OB_DECL_BUTTON(MouseButton,X3);		//!< 拡張3ボタン
		OB_DECL_BUTTON(MouseButton,X4);		//!< 拡張4ボタン
		OB_DECL_AXIS(MouseAxis,Wheel);		//!< ホイール
		OB_DECL_AXIS(MouseAxis,WheelX);		//!< ホイールX
		OB_DECL_AXIS(MouseAxis,X);			//!< 座標X
		OB_DECL_AXIS(MouseAxis,Y);			//!< 座標Y
		OB_DECL_AXIS(MouseAxis,DeltaX);		//!< 移動量X
		OB_DECL_AXIS(MouseAxis,DeltaY);		//!< 移動量Y

	};

}// namespcae ob::input