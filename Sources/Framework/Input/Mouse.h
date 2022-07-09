//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Input.h>
#include <Framework/Core/Hash/crc32.h>

namespace ob::input {

#define OB_DECL_INPUT(name) static constexpr Input name{ Code,MouseButton::name };

	
	class Mouse {
	public:
		static constexpr u32 Code{ OB_FNV32("Mouse") };
	public:

		OB_DECL_INPUT(Left);		//!< 左ボタン
		OB_DECL_INPUT(Right);		//!< 右ボタン
		OB_DECL_INPUT(Middle);		//!< 中ボタン
		OB_DECL_INPUT(X1);			//!< 拡張1ボタン
		OB_DECL_INPUT(X2);			//!< 拡張21ボタン
		OB_DECL_INPUT(X3);			//!< 拡張3ボタン
		OB_DECL_INPUT(X4);			//!< 拡張4ボタン
		OB_DECL_INPUT(Wheel);		//!< ホイール
		OB_DECL_INPUT(Position);	//!< 位置
		OB_DECL_INPUT(Move);		//!< 移動量

	};

#undef OB_DECL_INPUT

}// namespcae ob::input