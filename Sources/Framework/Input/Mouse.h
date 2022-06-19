//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Duration.h>
#include <Framework/Input/InputType.h>

namespace ob::input {

	class MouseInput {
	public:

		constexpr MouseInput(MouseButton button)
			:m_button(button)
		{}
		Vec2 value()const;
	//	bool down()const;

	//	template<class T,typename... Args>
	//	void bindDown(Handle& handle,T& instance, void(T::* pMethod)(Args...)const) {
	//		bindButton(MouseButton::Left,handle,Delegate(instance,pMethod));
	//	}
	//private:
	//	void bindButton(MouseButton,Handle&, const Notifier::delegate_type& );
	private:
		MouseButton m_button;
	};

	class Mouse {
	public:
		static constexpr MouseInput DeltaPos{ MouseButton::Left };
		static constexpr MouseInput Pos{ MouseButton::Left };
	};

}// namespcae ob::input