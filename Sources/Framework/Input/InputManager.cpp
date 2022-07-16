//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/InputManager.h>
#include <Framework/Input/Private/KeyboardDevice.h>
#include <Framework/Input/Private/MouseDevice.h>
#include <Framework/Core/Hash/crc32.h>

namespace ob::input{

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class InputManagerImpl {
    public:

        InputManagerImpl() {
            m_deviceList.emplace_back(std::make_unique<KeyboardDevice>());
            m_deviceList.emplace_back(std::make_unique<MouseDevice>());
        }

        //@―---------------------------------------------------------------------------
        //! @brief  更新
        //@―---------------------------------------------------------------------------
        void update() {
            for (auto& device : m_deviceList) {
                device->update();
            }
        }

        //@―---------------------------------------------------------------------------
        //! @brief  デバイスを取得
        //@―---------------------------------------------------------------------------
        IInputDevice* findDevice(u32 guid, u32 user) {
            for (auto& device : m_deviceList) {
                if (device->getDeviceId() == guid) {
                    return device.get();
                }
            }
            return nullptr;
        }

    private:

        List<std::unique_ptr<IInputDevice>> m_deviceList;

    };


    InputManager::InputManager() = default;
    InputManager::~InputManager() = default;
    void InputManager::update() { m_impl->update(); }
    IInputDevice* InputManager::findDevice(u32 guid,u32 user) {return  m_impl->findDevice(guid,user); }

}// namespace ob