//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Button.h>
#include <Framework/Core/Hash/Hash.h>

namespace ob::input {

#define OB_DECL_BUTTON(name) static constexpr Button name{ Code,Key::name };

	class Keyboard {
	public:

		static constexpr u32 Code{ OB_HASH32("Keyboard")};	//!< デバイスコード

		//@―---------------------------------------------------------------------------
		//! @brief      ボタン取得
		//@―---------------------------------------------------------------------------
		static constexpr Button GetButton(Key key) { return {Code,key}; }

	public:
		OB_DECL_BUTTON(A);
		OB_DECL_BUTTON(B);
		OB_DECL_BUTTON(C);
		OB_DECL_BUTTON(D);
		OB_DECL_BUTTON(E);
		OB_DECL_BUTTON(F);
		OB_DECL_BUTTON(G);
		OB_DECL_BUTTON(H);
		OB_DECL_BUTTON(I);
		OB_DECL_BUTTON(J);
		OB_DECL_BUTTON(K);
		OB_DECL_BUTTON(L);
		OB_DECL_BUTTON(M);
		OB_DECL_BUTTON(N);
		OB_DECL_BUTTON(O);
		OB_DECL_BUTTON(P);
		OB_DECL_BUTTON(Q);
		OB_DECL_BUTTON(R);
		OB_DECL_BUTTON(S);
		OB_DECL_BUTTON(T);
		OB_DECL_BUTTON(U);
		OB_DECL_BUTTON(V);
		OB_DECL_BUTTON(W);
		OB_DECL_BUTTON(X);
		OB_DECL_BUTTON(Y);
		OB_DECL_BUTTON(Z);
		OB_DECL_BUTTON(LeftArrow);
		OB_DECL_BUTTON(RightArrow);
		OB_DECL_BUTTON(UpArrow);
		OB_DECL_BUTTON(DownArrow);
		OB_DECL_BUTTON(Escape);
	};

#undef OB_DECL_BUTTON

}// namespcae ob::input