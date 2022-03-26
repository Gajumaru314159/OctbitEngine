//***********************************************************
//! @file
//! @brief		デバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/System.h>

namespace ob::graphic
{

    //@―---------------------------------------------------------------------------
    //! @brief  デバイスを取得
    //@―---------------------------------------------------------------------------
    IDevice* Device::Get() {
        auto pDevice = System::Instance().getDevice();
        OB_CHECK_ASSERT(pDevice, "ob::graphic::Systemが初期化されていないためデバイスの取得に失敗しました。");
        return pDevice;
    }

}// namespace ob