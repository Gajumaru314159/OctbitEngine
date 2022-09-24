//***********************************************************
//! @file
//! @brief		入力マネージャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/InputManager.h>

#include <Framework/Input/Device/KeyboardDevice.h>
#include <Framework/Input/Device/MouseDevice.h>
#include <Framework/Input/Device/DirectInputDevice.h>

#include <Framework/Input/Settings.h>

#include <Framework/Engine/Engine.h>
#include <Framework/Engine/ModuleFactory.h>

namespace ob::input{

    InputModule::InputModule()
    {
        auto& settings = GEngine->ini().get<Settings>();

        // キーボード
        if(settings.useKeyboard) {
            auto device = std::make_unique<KeyboardDevice>();
            DeviceKey key{ device->getDeviceId() ,0 };
            m_devices[key] = std::move(device);
        }
        // マウス
        if (settings.useMouse) {
            auto device = std::make_unique<MouseDevice>();
            DeviceKey key{ device->getDeviceId() ,0 };
            m_devices[key] = std::move(device);
        }
    }
    InputModule::~InputModule() = default;
    void InputModule::update() {

        for (auto& [id, device] : m_devices) {
            device->update();
        }
    
    }
    IInputDevice* InputModule::findDevice(DeviceID id,u32 user) {

        auto found = m_devices.find({ id,user });
        if (found == m_devices.end())return nullptr;
        return found->second.get();
    
    }

}// namespace ob

REGISTER_MODULE(ob::input::InputModule);

void Link_Input() {

}