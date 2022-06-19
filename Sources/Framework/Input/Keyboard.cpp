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

	//@―---------------------------------------------------------------------------
	//! @berif	キーが押されているか
	//@―---------------------------------------------------------------------------
	bool KeyboardInput::down()const {
		return InputManager::Instance().m_impl->down(m_key);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	キーが離されているか
	//@―---------------------------------------------------------------------------
	bool KeyboardInput::up()const {
		return InputManager::Instance().Down(m_key);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	キーが押された瞬間か
	//@―---------------------------------------------------------------------------
	bool KeyboardInput::pressed()const {
		return InputManager::Instance().Down(m_key);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	キーが離された瞬間か
	//@―---------------------------------------------------------------------------
	bool KeyboardInput::released()const {
		return InputManager::Instance().Down(m_key);
	}

	//@―---------------------------------------------------------------------------
	//! @berif	バインド
	//@―---------------------------------------------------------------------------
	void KeyboardInput::bindButtonImpl(TriggerType type, Key key, ButtonHandle& handle, const ButtonDelegate& func)const{
		InputManager::Instance().m_impl->bindButton(type,key,handle,func);
	}

}// namespace ob::input