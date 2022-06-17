//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Duration.h>
#include <Framework/Input/InputType.h>

namespace ob::input {

	class KeyboardInput {
	public:
		constexpr KeyboardInput(Key key)
			: m_key(key)
		{}

		bool down()const;

		template<class T>
		void bindDown(ButtonHandle& handle, T& instance, void(T::* pMethod)()const)const {
			bindButtonImpl(TriggerType::Down, m_key, handle, Delegate(instance, pMethod));
		}

		void bindDown(ButtonHandle& handle, const ButtonDelegate& func)const {
			bindButtonImpl(TriggerType::Down, m_key, handle, func);
		}

	private:
		void bindButtonImpl(TriggerType, Key, ButtonHandle&, const ButtonDelegate&)const;
	private:
		Key m_key;
	};


	namespace Keyboard {
		inline constexpr KeyboardInput W{ Key::W };
		inline constexpr KeyboardInput A{ Key::A };
		inline constexpr KeyboardInput S{ Key::S };
		inline constexpr KeyboardInput D{ Key::D };
	};




}// namespcae ob::input