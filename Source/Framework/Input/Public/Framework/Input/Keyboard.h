//***********************************************************
//! @file
//! @brief		キーボード
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Button.h>
#include <Framework/Core/Hash/Hash.h>

namespace ob::input {

	class Keyboard {
		OB_DECL_INPUT_DEVICE_ID("Keyboard");
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      ボタン取得
		//@―---------------------------------------------------------------------------
		static constexpr Button GetButton(Key key) { return {Code,key}; }

	public:
		OB_DECL_BUTTON(Key,A);
		OB_DECL_BUTTON(Key,B);
		OB_DECL_BUTTON(Key,C);
		OB_DECL_BUTTON(Key,D);
		OB_DECL_BUTTON(Key,E);
		OB_DECL_BUTTON(Key,F);
		OB_DECL_BUTTON(Key,G);
		OB_DECL_BUTTON(Key,H);
		OB_DECL_BUTTON(Key,I);
		OB_DECL_BUTTON(Key,J);
		OB_DECL_BUTTON(Key,K);
		OB_DECL_BUTTON(Key,L);
		OB_DECL_BUTTON(Key,M);
		OB_DECL_BUTTON(Key,N);
		OB_DECL_BUTTON(Key,O);
		OB_DECL_BUTTON(Key,P);
		OB_DECL_BUTTON(Key,Q);
		OB_DECL_BUTTON(Key,R);
		OB_DECL_BUTTON(Key,S);
		OB_DECL_BUTTON(Key,T);
		OB_DECL_BUTTON(Key,U);
		OB_DECL_BUTTON(Key,V);
		OB_DECL_BUTTON(Key,W);
		OB_DECL_BUTTON(Key,X);
		OB_DECL_BUTTON(Key,Y);
		OB_DECL_BUTTON(Key,Z);
		OB_DECL_BUTTON(Key,LeftArrow);
		OB_DECL_BUTTON(Key,RightArrow);
		OB_DECL_BUTTON(Key,UpArrow);
		OB_DECL_BUTTON(Key,DownArrow);
		OB_DECL_BUTTON(Key,Escape);
	};

}// namespcae ob::input