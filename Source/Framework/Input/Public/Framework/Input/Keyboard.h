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
	public:
		OB_DECL_INPUT_DEVICE_ID("Keyboard");

		//@―---------------------------------------------------------------------------
		//! @brief      ボタン取得
		//@―---------------------------------------------------------------------------
		static constexpr Button GetButton(Key key) { return {ID,key}; }

	public:
        OB_DECL_BUTTON(Key, F1);				//!< F1
        OB_DECL_BUTTON(Key, F2);				//!< F2
        OB_DECL_BUTTON(Key, F3);				//!< F3
        OB_DECL_BUTTON(Key, F4);				//!< F4
        OB_DECL_BUTTON(Key, F5);				//!< F5
        OB_DECL_BUTTON(Key, F6);				//!< F6
        OB_DECL_BUTTON(Key, F7);				//!< F7
        OB_DECL_BUTTON(Key, F8);				//!< F8
        OB_DECL_BUTTON(Key, F9);				//!< F9
        OB_DECL_BUTTON(Key, F10);				//!< F10
        OB_DECL_BUTTON(Key, F11);				//!< F11
        OB_DECL_BUTTON(Key, F12);				//!< F12

        OB_DECL_BUTTON(Key, Keypad0);			//!< テンキー0
        OB_DECL_BUTTON(Key, Keypad1);			//!< テンキー1
        OB_DECL_BUTTON(Key, Keypad2);			//!< テンキー2
        OB_DECL_BUTTON(Key, Keypad3);			//!< テンキー3
        OB_DECL_BUTTON(Key, Keypad4);			//!< テンキー4
        OB_DECL_BUTTON(Key, Keypad5);			//!< テンキー5
        OB_DECL_BUTTON(Key, Keypad6);			//!< テンキー6
        OB_DECL_BUTTON(Key, Keypad7);			//!< テンキー7
        OB_DECL_BUTTON(Key, Keypad8);			//!< テンキー8
        OB_DECL_BUTTON(Key, Keypad9);			//!< テンキー9
        OB_DECL_BUTTON(Key, KeypadPeriod);		//!< テンキー.
        OB_DECL_BUTTON(Key, KeypadDivide);		//!< テンキー/
        OB_DECL_BUTTON(Key, KeypadMultiply);	//!< テンキー*
        OB_DECL_BUTTON(Key, KeypadMinus);		//!< テンキー-
        OB_DECL_BUTTON(Key, KeypadPlus);		//!< テンキー+
        OB_DECL_BUTTON(Key, KeypadEnter);		//!< テンキーEnter

        OB_DECL_BUTTON(Key, Alpha0);			//!< 数字キー0
        OB_DECL_BUTTON(Key, Alpha1);			//!< 数字キー1
        OB_DECL_BUTTON(Key, Alpha2);			//!< 数字キー2
        OB_DECL_BUTTON(Key, Alpha3);			//!< 数字キー3
        OB_DECL_BUTTON(Key, Alpha4);			//!< 数字キー4
        OB_DECL_BUTTON(Key, Alpha5);			//!< 数字キー5
        OB_DECL_BUTTON(Key, Alpha6);			//!< 数字キー6
        OB_DECL_BUTTON(Key, Alpha7);			//!< 数字キー7
        OB_DECL_BUTTON(Key, Alpha8);			//!< 数字キー8
        OB_DECL_BUTTON(Key, Alpha9);			//!< 数字キー9

        OB_DECL_BUTTON(Key, A);					//!< A
        OB_DECL_BUTTON(Key, B);					//!< B
        OB_DECL_BUTTON(Key, C);					//!< C
        OB_DECL_BUTTON(Key, D);					//!< D
        OB_DECL_BUTTON(Key, E);					//!< E
        OB_DECL_BUTTON(Key, F);					//!< F
        OB_DECL_BUTTON(Key, G);					//!< G
        OB_DECL_BUTTON(Key, H);					//!< H
        OB_DECL_BUTTON(Key, I);					//!< I
        OB_DECL_BUTTON(Key, J);					//!< J
        OB_DECL_BUTTON(Key, K);					//!< K
        OB_DECL_BUTTON(Key, L);					//!< L
        OB_DECL_BUTTON(Key, M);					//!< M
        OB_DECL_BUTTON(Key, N);					//!< N
        OB_DECL_BUTTON(Key, O);					//!< O
        OB_DECL_BUTTON(Key, P);					//!< P
        OB_DECL_BUTTON(Key, Q);					//!< Q
        OB_DECL_BUTTON(Key, R);					//!< R
        OB_DECL_BUTTON(Key, S);					//!< S
        OB_DECL_BUTTON(Key, T);					//!< T
        OB_DECL_BUTTON(Key, U);					//!< U
        OB_DECL_BUTTON(Key, V);					//!< V
        OB_DECL_BUTTON(Key, W);					//!< W
        OB_DECL_BUTTON(Key, X);					//!< X
        OB_DECL_BUTTON(Key, Y);					//!< Y
        OB_DECL_BUTTON(Key, Z);					//!< Z

        OB_DECL_BUTTON(Key, Exclaim);			//!< !
        OB_DECL_BUTTON(Key, DoubleQuote);		//!< "
        OB_DECL_BUTTON(Key, Hash);				//!< #
        OB_DECL_BUTTON(Key, Dollar);			//!< $
        OB_DECL_BUTTON(Key, Percent);			//!< %
        OB_DECL_BUTTON(Key, Ampersand);			//!< &
        OB_DECL_BUTTON(Key, Quote);				//!< '
        OB_DECL_BUTTON(Key, LeftParen);			//!< (
        OB_DECL_BUTTON(Key, RightParen);		//!< )
        OB_DECL_BUTTON(Key, Asterisk);			//!< *
        OB_DECL_BUTTON(Key, Plus);				//!< +
        OB_DECL_BUTTON(Key, Comma);				//!< )
        OB_DECL_BUTTON(Key, Minus);				//!< -
        OB_DECL_BUTTON(Key, Period);			//!< .
        OB_DECL_BUTTON(Key, Slash);				//!< /
        OB_DECL_BUTTON(Key, Colon);				//!< :
        OB_DECL_BUTTON(Key, Semicolon);			//!< ;
        OB_DECL_BUTTON(Key, Less);				//!< <
        OB_DECL_BUTTON(Key, Equals);			//!< =
        OB_DECL_BUTTON(Key, Greater);			//!< >
        OB_DECL_BUTTON(Key, Question);			//!< ?
        OB_DECL_BUTTON(Key, At);				//!< @
        OB_DECL_BUTTON(Key, LeftBracke);		//!< [
        OB_DECL_BUTTON(Key, Backslash);			//!< \ 
        OB_DECL_BUTTON(Key, RightBracket);		//!< ]
        OB_DECL_BUTTON(Key, Caret);				//!< ^
        OB_DECL_BUTTON(Key, Underscore);		//!< _
        OB_DECL_BUTTON(Key, BackQuote);			//!< `
        OB_DECL_BUTTON(Key, LeftCurlyBracket);	//!< {
        OB_DECL_BUTTON(Key, Pipe);				//!< |
        OB_DECL_BUTTON(Key, RightCurlyBracket);	//!< }
        OB_DECL_BUTTON(Key, Tilde);				//!< ~

        OB_DECL_BUTTON(Key, UpArrow);			//!< ↑
        OB_DECL_BUTTON(Key, DownArrow);			//!< ↓
        OB_DECL_BUTTON(Key, RightArrow);		//!< →
        OB_DECL_BUTTON(Key, LeftArrow);			//!< ←

        OB_DECL_BUTTON(Key, Escape);			//!< Escape
        OB_DECL_BUTTON(Key, Space);				//!< Space
        OB_DECL_BUTTON(Key, Tab);				//!< Tab
        OB_DECL_BUTTON(Key, Backspace);			//!< Backspace
        OB_DECL_BUTTON(Key, Delete);			//!< Delete
        OB_DECL_BUTTON(Key, Enter);				//!< Enter / Return
        OB_DECL_BUTTON(Key, Insert);			//!< Insert
        OB_DECL_BUTTON(Key, CapsLosk);			//!< CapsLock
        OB_DECL_BUTTON(Key, PrintScreen);		//!< PrintScreen
        OB_DECL_BUTTON(Key, ScrollLock);		//!< ScrollLock

        OB_DECL_BUTTON(Key, Pause);				//!< Pause / Break
        OB_DECL_BUTTON(Key, Home);				//!< Home
        OB_DECL_BUTTON(Key, End);				//!< End
        OB_DECL_BUTTON(Key, PageUp);				//!< PageUp
        OB_DECL_BUTTON(Key, PageDown);			//!< PageDown
        OB_DECL_BUTTON(Key, Numlock);			//!< NumLock

        OB_DECL_BUTTON(Key, Shift);				//!< Shift
        OB_DECL_BUTTON(Key, LeftShilt);			//!< 左Shift
        OB_DECL_BUTTON(Key, RightShift);		//!< 右Shift

        OB_DECL_BUTTON(Key, Ctrl);				//!< Ctrl
        OB_DECL_BUTTON(Key, LeftCtrl);			//!< 左Ctrl
        OB_DECL_BUTTON(Key, RightCtrl);			//!< 右Ctrl

        OB_DECL_BUTTON(Key, Alt);				//!< Alt
        OB_DECL_BUTTON(Key, LeftAlt);			//!< 左Alt
        OB_DECL_BUTTON(Key, RightAlt);			//!< 右Alt

        OB_DECL_BUTTON(Key, Command);			//!< Command
        OB_DECL_BUTTON(Key, LeftCommand);		//!< 左Command
        OB_DECL_BUTTON(Key, RightCommand);		//!< 右Command

        OB_DECL_BUTTON(Key, Apple);				//!< Appleキー
        OB_DECL_BUTTON(Key, LeftApple);			//!< 左Appleキー
        OB_DECL_BUTTON(Key, RightApple);		//!< 右Appleキー

        OB_DECL_BUTTON(Key, Windows);			//!< Windowsキー
        OB_DECL_BUTTON(Key, LeftWindows);		//!< 左Windowsキー
        OB_DECL_BUTTON(Key, RightWindows);		//!< 右Windowsキー
	};

}// namespcae ob::input