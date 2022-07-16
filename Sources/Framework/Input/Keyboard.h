//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Button.h>
#include <Framework/Core/Hash/crc32.h>

namespace ob::input {

#define OB_DECL_BUTTON(name) static constexpr Button name{ Code,Key::name };

	class Keyboard {
	public:
		static constexpr u32 Code{ OB_FNV32("Keyboard")};
	public:
		OB_DECL_BUTTON(W);
		OB_DECL_BUTTON(S);
		OB_DECL_BUTTON(A);
		OB_DECL_BUTTON(D);
	};

#undef OB_DECL_BUTTON

}// namespcae ob::input