//***********************************************************
//! @file
//! @brief		グラフィック・デバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Swapchain.h"
#include "CommandList.h"

namespace ob {
    namespace graphic {

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
            //! @brief  スワップチェーンの生成
            //@―---------------------------------------------------------------------------
            Ref<Swapchain> createSwapchain(const SwapchainDesc& desc);

            //@―---------------------------------------------------------------------------
            //! @brief  コマンドリストの生成
            //@―---------------------------------------------------------------------------
            Ref<CommandList> createCommandList(CommandListType type);

        protected:

            virtual Ref<Swapchain> createSwapchainImpl(const SwapchainDesc& desc) = 0;
            virtual Ref<CommandList> createCommandListImpl(CommandListType type)=0;

        };

    }// namespace graphic
}// namespcae ob