//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/SwapChain.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    //! @brief  生成
    Ref<SwapChain> SwapChain::Create(const SwapChainDesc& desc) {
        if (auto rhi= Device::Get()) {
            return rhi->createSwapChain(desc);
        }
        return nullptr;
    }

}