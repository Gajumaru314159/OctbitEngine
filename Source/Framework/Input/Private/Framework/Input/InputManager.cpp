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

    static InputModule* s_instance = nullptr;
    
    InputModule* InputModule::Get() {
        return s_instance;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    InputModule::InputModule(platform::WindowManager&)
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
            auto device = std::make_unique<MouseDevice>(platform::Window::Main());
            DeviceKey key{ device->getDeviceId() ,0 };
            m_devices[key] = std::move(device);
        }

        OB_ASSERT(s_instance == nullptr, "{}は既に生成されています。", TypeId::Get<decltype(this)>().name());
        s_instance = this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    InputModule::~InputModule() {
        s_instance = nullptr;
    }


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