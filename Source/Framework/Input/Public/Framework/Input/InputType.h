//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Hash/Hash.h>

namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @brief	デバイスID
	//@―---------------------------------------------------------------------------
	enum class DeviceID : u32;
#define	OB_DEVICE_ID(name) DeviceID{OB_HASH32(name)}
#define OB_DECL_INPUT_DEVICE_ID(name) static constexpr DeviceID ID{ OB_DEVICE_ID(name) }

    //@―---------------------------------------------------------------------------
    //! @brief  キーコード
	//! 
	//! @details	キーボードで入力することのできるコード一覧。
	//! @ref		MouseButton
	//! @ref		PadButtton
    //@―---------------------------------------------------------------------------
    enum class Key :u32 {
		Unknown = 0,		//!< 不明

		F1,					//!< F1
		F2,					//!< F2
		F3,					//!< F3
		F4,					//!< F4
		F5,					//!< F5
		F6,					//!< F6
		F7,					//!< F7
		F8,					//!< F8
		F9,					//!< F9
		F10,				//!< F10
		F11,				//!< F11
		F12,				//!< F12

		Keypad0,			//!< テンキー0
		Keypad1,			//!< テンキー1
		Keypad2,			//!< テンキー2
		Keypad3,			//!< テンキー3
		Keypad4,			//!< テンキー4
		Keypad5,			//!< テンキー5
		Keypad6,			//!< テンキー6
		Keypad7,			//!< テンキー7
		Keypad8,			//!< テンキー8
		Keypad9,			//!< テンキー9
		KeypadPeriod,		//!< テンキー.
		KeypadDivide,		//!< テンキー/
		KeypadMultiply,		//!< テンキー*
		KeypadMinus,		//!< テンキー-
		KeypadPlus,			//!< テンキー+
		KeypadEnter,		//!< テンキーEnter

		Alpha0,				//!< 数字キー0
		Alpha1,				//!< 数字キー1
		Alpha2,				//!< 数字キー2
		Alpha3,				//!< 数字キー3
		Alpha4,				//!< 数字キー4
		Alpha5,				//!< 数字キー5
		Alpha6,				//!< 数字キー6
		Alpha7,				//!< 数字キー7
		Alpha8,				//!< 数字キー8
		Alpha9,				//!< 数字キー9

		A,					//!< A
		B,					//!< B
		C,					//!< C
		D,					//!< D
		E,					//!< E
		F,					//!< F
		G,					//!< G
		H,					//!< H
		I,					//!< I
		J,					//!< J
		K,					//!< K
		L,					//!< L
		M,					//!< M
		N,					//!< N
		O,					//!< O
		P,					//!< P
		Q,					//!< Q
		R,					//!< R
		S,					//!< S
		T,					//!< T
		U,					//!< U
		V,					//!< V
		W,					//!< W
		X,					//!< X
		Y,					//!< Y
		Z,					//!< Z

		Exclaim,			//!< !
		DoubleQuote,		//!< "
		Hash,				//!< #
		Dollar,				//!< $
		Percent,			//!< %
		Ampersand,			//!< &
		Quote,				//!< '
		LeftParen,			//!< (
		RightParen,			//!< )
		Asterisk,			//!< *
		Plus,				//!< +
		Comma,				//!< ,
		Minus,				//!< -
		Period,				//!< .
		Slash,				//!< /
		Colon,				//!< :
		Semicolon,			//!< ;
		Less,				//!< <
		Equals,				//!< =
		Greater,			//!< >
		Question,			//!< ?
		At,					//!< @
		LeftBracke,			//!< [
		Backslash,			//!< \ または ＼
		RightBracket,		//!< ]
		Caret,				//!< ^
		Underscore,			//!< _
		BackQuote,			//!< `
		LeftCurlyBracket,	//!< {
		Pipe,				//!< |
		RightCurlyBracket,	//!< }
		Tilde,				//!< ~

		UpArrow,			//!< ↑
		DownArrow,			//!< ↓
		RightArrow,			//!< →
		LeftArrow,			//!< ←
		
		Escape,				//!< Escape
		Space,				//!< Space
		Tab,				//!< Tab
		Backspace,			//!< Backspace
		Delete,				//!< Delete
		Enter,				//!< Enter / Return
		Insert,				//!< Insert
		CapsLosk,			//!< CapsLock
		PrintScreen,		//!< PrintScreen
		ScrollLock,			//!< ScrollLock

		Pause,				//!< Pause / Break
		Home,				//!< Home
		End,				//!< End
		PageUp,				//!< PageUp
		PageDown,			//!< PageDown
		Numlock,			//!< NumLock
		
		Shift,				//!< Shift
		LeftShilt,			//!< 左Shift
		RightShift,			//!< 右Shift

		Ctrl,				//!< Ctrl
		LeftCtrl,			//!< 左Ctrl
		RightCtrl,			//!< 右Ctrl

		Alt,				//!< Alt
		LeftAlt,			//!< 左Alt
		RightAlt,			//!< 右Alt

		Command,			//!< Command
		LeftCommand,		//!< 左Command
		RightCommand,		//!< 右Command

		Apple,				//!< Appleキー
		LeftApple,			//!< 左Appleキー
		RightApple,			//!< 右Appleキー

		Windows,			//!< Windowsキー
		LeftWindows,		//!< 左Windowsキー
		RightWindows,		//!< 右Windowsキー

		MAX					//!< 最大
    };


	//@―---------------------------------------------------------------------------
	//! @brief		修飾キー
	//@―---------------------------------------------------------------------------
	enum class ModifierKey :u32 {
		None	= 0x0000,	//!< なし
		Alt		= 0x0001,	//!< Altキー
		Shift	= 0x0002,	//!< Shiftキー
		Control	= 0x0004,	//!< Ctrlキー
	};


	//@―---------------------------------------------------------------------------
	//! @brief		マウスボタン
	//@―---------------------------------------------------------------------------
	enum class MouseButton :u32 {
		Left,		//!< 左ボタン
		Right,		//!< 右ボタン
		Middle,		//!< 中ボタン
		X1,			//!< 拡張ボタン1
		X2,			//!< 拡張ボタン2
		X3,			//!< 拡張ボタン3
		X4,			//!< 拡張ボタン4
	};

	//@―---------------------------------------------------------------------------
	//! @brief		マウス軸
	//@―---------------------------------------------------------------------------
	enum class MouseAxis :u32 {
		Wheel,		//!< ホイール
		WheelX,		//!< ホイールX
		X,			//!< 座標X
		Y,			//!< 座標Y
		DeltaX,		//!< 移動量X
		DeltaY,		//!< 移動量Y
	};



	//@―---------------------------------------------------------------------------
	//! @brief		入力状態
	//@―---------------------------------------------------------------------------
	enum class ButtonState {
		Down		= get_bit(0),	//!< 押された瞬間か
		Up			= get_bit(1),	//!< 離された瞬間か
		Pressed		= get_bit(2),	//!< 押されているか
		Released	= get_bit(3),	//!< 離されているか
	};

	//@―---------------------------------------------------------------------------
	//! @brief		入力状態フラグ
	//@―---------------------------------------------------------------------------
	using ButtonStates = BitFlags<ButtonState>;


	//@―---------------------------------------------------------------------------
	//! @brief		入力イベント・ノティファイア
	//@―---------------------------------------------------------------------------
	using ButtonNotifier = EventNotifier<>;
	//@―---------------------------------------------------------------------------
	//! @brief		入力イベント・ハンドル
	//@―---------------------------------------------------------------------------
	using ButtonHandle = ButtonNotifier::Handle;
	//@―---------------------------------------------------------------------------
	//! @brief		入力イベント・デリゲート
	//@―---------------------------------------------------------------------------
	using ButtonDelegate = ButtonNotifier::delegate_type;


	//@―---------------------------------------------------------------------------
	//! @brief		入力イベント・ノティファイア
	//@―---------------------------------------------------------------------------
	using AxisNotifier = EventNotifier<f32>;
	//@―---------------------------------------------------------------------------
	//! @brief		入力イベント・ハンドル
	//@―---------------------------------------------------------------------------
	using AxisHandle = AxisNotifier::Handle;
	//@―---------------------------------------------------------------------------
	//! @brief		入力イベント・デリゲート
	//@―---------------------------------------------------------------------------
	using AxisDelegate = AxisNotifier::delegate_type;

}// namespcae ob