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
        if (auto device = Device::Get()) {
            return device->createDisplay(desc);
        }
        return nullptr;
    }

}// namespace ob::rhi