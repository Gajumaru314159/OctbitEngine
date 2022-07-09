//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Input.h>
#include <Framework/Core/Hash/crc32.h>

namespace ob::input {

#define OB_DECL_INPUT(name) static constexpr Input name{ Code,Key::name };


	class Keyboard {
	public:
		static constexpr u32 Code{ OB_FNV32("Keyboard")};
	public:
		OB_DECL_INPUT(W);
		OB_DECL_INPUT(S);
		OB_DECL_INPUT(A);
		OB_DECL_INPUT(D);
	};

#undef OB_DECL_INPUT

}// namespcae ob::input