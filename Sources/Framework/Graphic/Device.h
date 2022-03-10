//***********************************************************
//! @file
//! @brief		グラフィック・デバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Swapchain.h"
#include "CommandList.h"

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  グラフィック・デバイス
    //@―---------------------------------------------------------------------------
    class Device {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        virtual ~Device() = default;

        //@―---------------------------------------------------------------------------
        //! @brief  スワップチェーンを生成
        //@―---------------------------------------------------------------------------
        Ref<Swapchain> CreateSwapchain(const SwapchainDesc& desc);

        //@―---------------------------------------------------------------------------
        //! @brief  コマンドリストを生成
        //@―---------------------------------------------------------------------------
        Ref<CommandList> CreateCommandList(CommandListType type);

    protected:

        virtual Ref<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) = 0;
        virtual Ref<CommandList> CreateCommandListImpl(CommandListType type) = 0;

    };

}// namespace pb::graphic