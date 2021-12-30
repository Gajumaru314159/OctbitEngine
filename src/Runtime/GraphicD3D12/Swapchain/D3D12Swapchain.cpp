//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "D3D12Swaphain.h"
#include "../Utility/D3D12Utility.h"

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        D3D12Swapchain::D3D12Swapchain(
            HWND hWnd,
            ComPtr<IDXGIFactory4> dxgiFactory,
            ComPtr<ID3D12CommandQueue> commandQueue,
            const SwapchainDesc& desc){

            OB_REQUIRE(hWnd!=NULL);
            OB_REQUIRE(dxgiFactory);
            OB_REQUIRE(commandQueue);

            DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
            swapchainDesc.Width = desc.width;										// 画面解像度【横】
            swapchainDesc.Height = desc.height;										// 画面解像度【縦】
            swapchainDesc.Format = D3D12Utility::convertTextureFormat(desc.format);	// ピクセルフォーマット
            swapchainDesc.Stereo = false;											// 3Dディスプレイかどうか
            swapchainDesc.SampleDesc.Count = desc.sampleCount;                      // マルチサンプル指定
            swapchainDesc.SampleDesc.Quality = desc.sampleQuarity;                  // マルチサンプル指定
            swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;						// CPUのバックバッファへのアクセス方法
            swapchainDesc.BufferCount = desc.bufferCount;							// バッファの数。ダブルバッファのため2。
            swapchainDesc.Scaling = DXGI_SCALING_STRETCH;							// リサイズ時の挙動。見た目に合わせる。
            swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;				// スワップ後に速やかに破棄
            swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;					// バックバッファの透過の挙動
            swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;			// ウィンドウ⇔フルスクリーン切り替え可能

            auto result = dxgiFactory->CreateSwapChainForHwnd(
                commandQueue.Get(),
                hWnd,
                &swapchainDesc,
                nullptr,  // フルスクリーン時の設定
                nullptr,  // アウトプット
                (IDXGISwapChain1**)m_swapchain.ReleaseAndGetAddressOf());

            OB_ENSURE(SUCCEEDED(result));
        }


    }// namespace graphic
}// namespace ob