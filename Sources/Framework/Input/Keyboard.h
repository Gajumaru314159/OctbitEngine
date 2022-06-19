//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Duration.h>
#include <Framework/Input/InputType.h>

namespace ob::input {

#define OB_DECL_INPUT_BIND(type)															\
	template<class T>																		\
	void bind##type(ButtonHandle& handle, T& instance, void(T::* pMethod)()const)const {	\
		bindButtonImpl(TriggerType::type, m_key, handle, Delegate(instance, pMethod));		\
	}																						\
	void bind##type(ButtonHandle& handle, const ButtonDelegate& func)const {				\
		bindButtonImpl(TriggerType::type, m_key, handle, func);								\
	}

	//@―---------------------------------------------------------------------------
	//! @brief	キーボードの入力イベント
	//@―---------------------------------------------------------------------------
	class KeyboardInput {
	public:

		//@―---------------------------------------------------------------------------
		//! @berif	コンストラクタ
		//@―---------------------------------------------------------------------------
		constexpr KeyboardInput(Key key)
			: m_key(key)
		{}

		//@―---------------------------------------------------------------------------
		//! @berif	キーが押されているか
		//@―---------------------------------------------------------------------------
		bool down()const;

		//@―---------------------------------------------------------------------------
		//! @berif	キーが離されているか
		//@―---------------------------------------------------------------------------
		bool up()const;

		//@―---------------------------------------------------------------------------
		//! @berif	キーが押された瞬間か
		//@―---------------------------------------------------------------------------
		bool pressed()const;

		//@―---------------------------------------------------------------------------
		//! @berif	キーが離された瞬間か
		//@―---------------------------------------------------------------------------
		bool released()const;


		//===============================================================
		//	バインドイベント
		//===============================================================
		OB_DECL_INPUT_BIND(Down);
		OB_DECL_INPUT_BIND(Up);
		OB_DECL_INPUT_BIND(Pressed);
		OB_DECL_INPUT_BIND(Released);

	private:
		void bindButtonImpl(TriggerType, Key, ButtonHandle&, const ButtonDelegate&)const;
	private:
		Key m_key;
	};


	class Keyboard {
	public:
		static constexpr KeyboardInput W{ Key::W };
		static constexpr KeyboardInput A{ Key::A };
		static constexpr KeyboardInput S{ Key::S };
		static constexpr KeyboardInput D{ Key::D };
	};




}// namespcae ob::input