//***********************************************************
//! @file
//! @brief		ディスプレイ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Display.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  生成
    //@―---------------------------------------------------------------------------
    Ref<Display> Display::Create(const DisplayDesc& desc) {
        return Device::Get()->createDisplay(desc);
    }

}// namespace ob::rhi