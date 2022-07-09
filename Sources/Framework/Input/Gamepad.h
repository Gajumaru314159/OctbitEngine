//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Input.h>

namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @brief	キーボードの入力イベント
	//@―---------------------------------------------------------------------------
	class GamePadInput :public Input {
	public:

		//@―---------------------------------------------------------------------------
		//! @berif	コンストラクタ
		//@―---------------------------------------------------------------------------
		constexpr GamePadInput(Key key)
			: Input(enum_cast(key)), m_key(key)
		{}

	private:
		Key m_key;
	};


	class GamePad {
	public:
		static constexpr GamePadInput A{ Key::W };
		static constexpr GamePadInput B{ Key::A };
		static constexpr GamePadInput X{ Key::S };
		static constexpr GamePadInput Y{ Key::D };
		static constexpr GamePadInput U{ Key::D };
		static constexpr GamePadInput D{ Key::D };
		static constexpr GamePadInput L{ Key::D };
		static constexpr GamePadInput R{ Key::D };
		static constexpr GamePadInput L1{ Key::D };
		static constexpr GamePadInput L2{ Key::D };
		static constexpr GamePadInput L3{ Key::D };
		static constexpr GamePadInput R1{ Key::D };
		static constexpr GamePadInput R2{ Key::D };
		static constexpr GamePadInput R3{ Key::D };
	};




}// namespcae ob::input