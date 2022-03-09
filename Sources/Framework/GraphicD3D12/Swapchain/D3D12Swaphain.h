//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Public/Swapchain.h>
#include "../Device/D3D12Device.h"

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        class D3D12Swapchain:public Swapchain{
        public:

            //===============================================================
            // コンストラクタ / デストラクタ
            //===============================================================

            //@―---------------------------------------------------------------------------
            //! @brief  説明
            //@―---------------------------------------------------------------------------
            D3D12Swapchain(
                HWND hWnd,
                ComPtr<IDXGIFactory4> dxgiFactory,
                ComPtr<ID3D12CommandQueue> commandQueue,
                const SwapchainDesc& desc);

        private:

            ComPtr<IDXGISwapChain4> m_swapchain = nullptr;  //<! スワップチェイン


        };

    }
}// namespcae ob