//***********************************************************
//! @file
//! @brief		グラフィック・デバイス
//! @author		Gajumaru
//***********************************************************
#include <Runtime/Graphic/Public/Device.h>

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief  スワップチェーンの生成
        //@―---------------------------------------------------------------------------
        Ref<Swapchain> Device::createSwapchain(const SwapchainDesc& desc) {
            return createSwapchainImpl(desc);
        }


        //@―---------------------------------------------------------------------------
        //! @brief  コマンドリストの生成
        //@―---------------------------------------------------------------------------
        Ref<CommandList> Device::createCommandList(CommandListType type) {
            return createCommandList(type);
        }


    }// namespace graphic
}// namespace ob