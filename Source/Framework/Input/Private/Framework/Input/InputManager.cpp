//***********************************************************
//! @file
//! @brief		入力マネージャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/InputManager.h>

#include <Framework/Input/Device/KeyboardDevice.h>
#include <Framework/Input/Device/MouseDevice.h>
#include <Framework/Input/Device/DirectInputDevice.h>

namespace ob::input{

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class InputManagerImpl {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        InputManagerImpl() {
            // キーボード
            {
                auto device = std::make_unique<KeyboardDevice>();
                DeviceKey key{ device->getDeviceId() ,0};
                m_devices[key] = std::move(device);
            }
            // マウス
            {
                auto device = std::make_unique<MouseDevice>();
                DeviceKey key{ device->getDeviceId() ,0 };
                m_devices[key] = std::move(device);
            }
            //// ゲームパッド
            //{
            //    auto device = std::make_unique<DirectInputDevice>();
            //    DeviceKey key{ device->getDeviceId() ,0 };
            //    m_devices[key] = std::move(device);
            //}
        }

        //@―---------------------------------------------------------------------------
        //! @brief  更新
        //@―---------------------------------------------------------------------------
        void update() {
            for (auto& [id,device] : m_devices) {
                device->update();
            }
        }

        //@―---------------------------------------------------------------------------
        //! @brief  デバイスを取得
        //@―---------------------------------------------------------------------------
        IInputDevice* findDevice(DeviceID id, u32 user) {
            auto found = m_devices.find({ id,user });
            if (found == m_devices.end())return nullptr;
            return found->second.get();
        }

    private:

        struct DeviceKey {
            DeviceID id{0};
            u32 user{0};
            bool operator==(const DeviceKey& rhs) const { return id == rhs.id && user == rhs.user; }
            bool operator!=(const DeviceKey& rhs) const { return !(*this == rhs); }
        };
        struct Hash {
            typedef std::size_t result_type;
            std::size_t operator()(const DeviceKey& key) const {
                return static_cast<size_t>(static_cast<size_t>(key.id) ^ (key.user<<28));
            }
        };

        using DevicePtr = UPtr<IInputDevice>;
        HashMap<DeviceKey, DevicePtr, Hash> m_devices;

    };


    InputManager::InputManager() = default;
    InputManager::~InputManager() = default;
    void InputManager::update() { m_impl->update(); }
    IInputDevice* InputManager::findDevice(DeviceID id,u32 user) {return  m_impl->findDevice(id,user); }

}// namespace ob