//***********************************************************
//! @file
//! @brief		グラフィック・デバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Device.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  スワップチェーンを生成
    //@―---------------------------------------------------------------------------
    Ref<Swapchain> Device::CreateSwapchain(const SwapchainDesc& desc) {
        return CreateSwapchainImpl(desc);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  コマンドリストを生成
    //@―---------------------------------------------------------------------------
    Ref<CommandList> Device::CreateCommandList(CommandListType type) {
        return CreateCommandListImpl(type);
    }


}// namespace pb::graphic