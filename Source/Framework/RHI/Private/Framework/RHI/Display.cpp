//***********************************************************
//! @file
//! @brief		ディスプレイ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Display.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  生成
    //@―---------------------------------------------------------------------------
    Ref<Display> Display::Create(const DisplayDesc& desc) {
        if (auto rhi= RHI::Get()) {
            return rhi->createDisplay(desc);
        }
        return nullptr;
    }

}