//***********************************************************
//! @file
//! @brief		入力マネージャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/InputManager.h>

#include <Framework/Input/Device/KeyboardDevice.h>
#include <Framework/Input/Device/MouseDevice.h>
#include <Framework/Input/Device/DirectInputDevice.h>

#include <Framework/Input/Config.h>

namespace ob::input{

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    InputModule::InputModule(Config* config, platform::WindowManager&) {

        if (config) {
            m_config = *config;
        }

        // キーボード
        if(m_config.useKeyboard) {
            auto device = std::make_unique<KeyboardDevice>();
            DeviceKey key{ device->getDeviceId() ,0 };
            m_devices[key] = std::move(device);
        }

    }

    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    InputModule::~InputModule() {
    }


    //@―---------------------------------------------------------------------------
    //! @brief  更新
    //@―---------------------------------------------------------------------------
    void InputModule::update() {
        if (m_mouseAdded == false && m_config.useMouse) {
            if (auto window = platform::Window::Main()) {

                auto device = std::make_unique<MouseDevice>(window);
                DeviceKey key{ device->getDeviceId() ,0 };
                m_devices[key] = std::move(device);
                
                m_mouseAdded = true;
            }
        }

        for (auto& [id, device] : m_devices) {
            device->update();
        }
    
    }

    //@―---------------------------------------------------------------------------
    //! @brief  デバイス取得
    //@―---------------------------------------------------------------------------
    IInputDevice* InputModule::findDevice(DeviceID id,u32 user) {

        auto found = m_devices.find({ id,user });
        if (found == m_devices.end())return nullptr;
        return found->second.get();
    
    }

}// namespace ob