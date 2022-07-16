//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Button.h>
#include <Framework/Input/Axis.h>
#include <Framework/Core/Hash/crc32.h>

namespace ob::input {

#define OB_DECL_BUTTON(name) static constexpr Button name{ Code,MouseButton::name };
#define OB_DECL_AXIS(name) static constexpr Axis name{ Code,MouseAxis::name };
	
	class Mouse {
	public:
		static constexpr u32 Code{ OB_FNV32("Mouse") };
	public:

		OB_DECL_BUTTON(Left);		//!< 左ボタン
		OB_DECL_BUTTON(Right);		//!< 右ボタン
		OB_DECL_BUTTON(Middle);		//!< 中ボタン
		OB_DECL_BUTTON(X1);			//!< 拡張1ボタン
		OB_DECL_BUTTON(X2);			//!< 拡張21ボタン
		OB_DECL_BUTTON(X3);			//!< 拡張3ボタン
		OB_DECL_BUTTON(X4);			//!< 拡張4ボタン
		OB_DECL_AXIS(Wheel);		//!< ホイール
		OB_DECL_AXIS(WheelX);		//!< ホイールX
		OB_DECL_AXIS(X);			//!< 座標X
		OB_DECL_AXIS(Y);			//!< 座標Y
		OB_DECL_AXIS(DeltaX);		//!< 移動量X
		OB_DECL_AXIS(DeltaY);		//!< 移動量Y

	};

#undef OB_DECL_BUTTON
#undef OB_DECL_AXIS

}// namespcae ob::input