//***********************************************************
//! @file
//! @brief		キーボード
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/Keyboard.h>
#include <Framework/Input/InputManager.h>
#include <Framework/Input/Private/KeyboardImpl.h>

namespace ob::input
{

	bool KeyboardInput::down()const {
		return InputManager::Instance().Down(m_key);
	}

	void KeyboardInput::bindButtonImpl(TriggerType type, Key key, ButtonHandle& handle, const ButtonDelegate& func)const{
		InputManager::Instance().m_impl->bindButton(type,key,handle,func);
	}



	/*
    //@―---------------------------------------------------------------------------
    //! @brief  押している間
    //@―---------------------------------------------------------------------------
	bool Keyboard::Down(Key key) {
		return InputManager::Instance().Down(key);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  離している間
	//@―---------------------------------------------------------------------------
	bool Keyboard::Up(Key key) {
		return InputManager::Instance().Up(key);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  押された瞬間
	//@―---------------------------------------------------------------------------
	bool Keyboard::Pressed(Key key) {
		return false;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  離された瞬間
	//@―---------------------------------------------------------------------------
	bool Keyboard::Released(Key key) {
		return false;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  押されている時間
	//@―---------------------------------------------------------------------------
	Duration Keyboard::PressedDuration(Key key) {
		return Duration(0);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  離されている時間
	//@―---------------------------------------------------------------------------
	Duration Keyboard::ReleasedDuration(Key key) {
		return Duration(DateTime(), DateTime::Now());
	}
	*/
}// namespace ob::input