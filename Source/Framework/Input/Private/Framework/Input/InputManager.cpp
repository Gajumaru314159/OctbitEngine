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

#include <Framework/Platform/WindowManager.h>

#include <Framework/Engine/Engine.h>

namespace ob::input{
    
    InputModule* InputModule::Get() {
        if (auto m = GEngine->get2<InputModule>()) {
            return m;
        }
        return nullptr;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    InputModule::InputModule(platform::WindowManager& windowManager)
    {
        //auto& config = GEngine->config().get<Config>();

        // キーボード
        //if(config.useKeyboard) {
        {
            auto device = std::make_unique<KeyboardDevice>();
            DeviceKey key{ device->getDeviceId() ,0 };
            m_devices[key] = std::move(device);
        }
        // マウス
        //if (config.useMouse) {
        {
            auto device = std::make_unique<MouseDevice>(windowManager.getMainWindow());
            DeviceKey key{ device->getDeviceId() ,0 };
            m_devices[key] = std::move(device);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    InputModule::~InputModule() = default;


    //@―---------------------------------------------------------------------------
    //! @brief  更新
    //@―---------------------------------------------------------------------------
    void InputModule::update() {

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