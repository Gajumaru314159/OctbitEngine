//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/InputManager.h>
#include <Framework/Input/Private/KeyboardImpl.h>
#include <Framework/Input/Private/MouseImpl.h>

namespace ob::input
{

    ////@―---------------------------------------------------------------------------
    ////! @brief  説明
    ////@―---------------------------------------------------------------------------
    //class InputManagerImpl {
    //public:

    //    void update() {
    //        m_keyboard.update();
    //    }

    ////private:
    //    KeyboardImpl m_keyboard;
    //};


    

    InputManager::InputManager() = default;
    InputManager::~InputManager() = default;
    void InputManager::update() {
        m_impl->update();
        m_mouse->update();
    }


    bool InputManager::Down(Key key) { return m_impl->down(key); }


}// namespace ob