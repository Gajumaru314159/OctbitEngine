﻿//***********************************************************
//! @file
//! @brief		入力マネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Interface/IInputDevice.h>
#include <Framework/Input/Config.h>

namespace ob::platform {
    class WindowManager;
}

namespace ob::input {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class InputModule : public Singleton<InputModule> {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        InputModule(Config*, platform::WindowManager&);
        ~InputModule();

        //@―---------------------------------------------------------------------------
        //! @brief  更新
        //@―---------------------------------------------------------------------------
        void update();

        //@―---------------------------------------------------------------------------
        //! @brief  入力デバイスを生成
        //@―---------------------------------------------------------------------------
        template<class T,typename... Args>
        IInputDevice* createDevice(Args&&... args) {
            OB_NOTIMPLEMENTED();
            return nullptr;
        }

        //@―---------------------------------------------------------------------------
        //! @brief  入力デバイスを探す
        //@―---------------------------------------------------------------------------
        IInputDevice* findDevice(DeviceID id,u32 user);

    private:

        struct DeviceKey {
            DeviceID id{ 0 };
            u32 user{ 0 };
            bool operator==(const DeviceKey& rhs) const { return id == rhs.id && user == rhs.user; }
            bool operator!=(const DeviceKey& rhs) const { return !(*this == rhs); }
        };
        struct Hash {
            std::size_t operator()(const DeviceKey& key) const {
                return static_cast<size_t>(static_cast<size_t>(key.id) ^ ((size_t)key.user << 28));
            }
        };

        using DevicePtr = UPtr<IInputDevice>;
        HashMap<DeviceKey, DevicePtr, Hash> m_devices;

        Config m_config;
        bool m_mouseAdded=false;
    };

}