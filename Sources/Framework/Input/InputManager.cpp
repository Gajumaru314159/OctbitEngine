//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/InputManager.h>
#include <Framework/Input/Private/KeyboardDevice.h>
#include <Framework/Input/Private/MouseImpl.h>
#include <Framework/Core/Hash/crc32.h>

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


    IInputDevice* InputManager::findDevice(u32 guid,u32 user) {
        if (guid == OB_FNV32("Keyboard")) {
            return m_impl.get();
        }
        if (guid == OB_FNV32("Mouse")) {
            //return m_mouse.get();
        }
        return nullptr;
    }


    

    InputManager::InputManager() = default;
    InputManager::~InputManager() = default;
    void InputManager::update() {
        m_impl->update();
        m_mouse->update();
    }


}// namespace ob