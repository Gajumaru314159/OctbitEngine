//***********************************************************
//! @file
//! @brief		入力マネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Input/Interface/IInputDevice.h>
#include <Framework/Engine/IModule.h>

namespace ob::input {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class InputModule:public engine::IModule{
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        InputModule();
        ~InputModule();

        //@―---------------------------------------------------------------------------
        //! @brief  更新
        //@―---------------------------------------------------------------------------
        void update()override;

        //@―---------------------------------------------------------------------------
        //! @brief  デバイスを探す
        //@―---------------------------------------------------------------------------
        IInputDevice* findDevice(DeviceID id,u32 user);

    public:

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

    };

}// namespcae ob