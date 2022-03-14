//***********************************************************
//! @file
//! @brief		スワップ・チェーン実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "SwapChainImpl.h"
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Framework/Graphic/Interface/ITexture.h>
#include <Framework/Platform/Window/WindowNativaAccessor.h>
#include "../Utility/Utility.h"

namespace {
    int static const s_maxSwapChainCount=4;
}

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    SwapChainImpl::SwapChainImpl(DeviceImpl& rDevice,const SwapchainDesc& desc) {

        OB_REQUIRE_EX(desc.window,"Windowがnullptrです。");

        m_desc = desc;
        HWND hWnd=static_cast<HWND>(ob::platform::WindowNativeAccessor::getHWND(*m_desc.window));

        auto size = desc.window->size();

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = size.width;									// 画面解像度【横】
        swapChainDesc.Height = size.height;								    // 画面解像度【縦】
        swapChainDesc.Format = Utility::convertTextureFormat(desc.format);	// ピクセルフォーマット
        swapChainDesc.Stereo = false;										// 3Dディスプレイかどうか
        swapChainDesc.SampleDesc.Count = 1;// desc.sampleCount;             // マルチサンプル指定
        swapChainDesc.SampleDesc.Quality = 1;// desc.sampleQuarity;         // マルチサンプル指定
        swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;					// CPUのバックバッファへのアクセス方法
        swapChainDesc.BufferCount = desc.backBufferNum;						// バッファの数。ダブルバッファのため2。
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;						// リサイズ時の挙動。見た目に合わせる。
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			// スワップ後に速やかに破棄
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;				// バックバッファの透過の挙動
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// ウィンドウ⇔フルスクリーン切り替え可能

        auto result = rDevice.getFactory()->CreateSwapChainForHwnd(
            rDevice.getCommandQueue().Get(),
            hWnd,
            &swapChainDesc,
            nullptr,  // フルスクリーン時の設定
            nullptr,  // アウトプット
            (IDXGISwapChain1**)m_swapchain.ReleaseAndGetAddressOf());

        OB_ENSURE(SUCCEEDED(result));

        initializeRenderTexture(rDevice);
    }




    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファの数を取得
    //@―---------------------------------------------------------------------------
    s32 SwapChainImpl::getBackBufferCount()const {
        return m_desc.backBufferNum;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  VSyncが有効か
    //@―---------------------------------------------------------------------------
    s32 SwapChainImpl::isVSyncEnabled()const {
        return m_desc.vsync;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  HDRが有効か
    //@―---------------------------------------------------------------------------
    s32 SwapChainImpl::isHdrEnabled()const {
        return m_desc.hdr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  描画先テクスチャを取得
    //@―---------------------------------------------------------------------------
    ITexture* SwapChainImpl::getRederTexture(s32 index)const {
        OB_NOTIMPLEMENTED();
        return nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファのサイズを変更
    //@―---------------------------------------------------------------------------
    bool SwapChainImpl::resizeBackBuffer(const Size& size) {
        OB_NOTIMPLEMENTED();
        return false;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //! 
    //! @details    表示するテクスチャを次のバックバッファにします。
    //@―---------------------------------------------------------------------------
    void SwapChainImpl::update() {
        OB_NOTIMPLEMENTED();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      レンダーテクスチャを初期化
    //@―---------------------------------------------------------------------------
    void SwapChainImpl::initializeRenderTexture(DeviceImpl& rDevice) {

        OB_REQUIRE_EX(
            m_desc.backBufferNum <= s_maxSwapChainCount,
            "{0}つのバックバッファは多すぎます。{1}以下にしてください。", m_desc.backBufferNum, s_maxSwapChainCount);
        OB_REQUIRE_EX(
            0 < m_desc.backBufferNum,
            "バックバッファは1つ以上にしてください。"backBufferNum);

        TextureDesc desc;
        desc.size = m_desc.size;
        desc.format = m_desc.format;
        //hdr

        // バックバッファを生成
        for (s32 i = 0; i < m_desc.backBufferNum; ++i) {
            m_buffers.push_back(rDevice.createTexture(desc, TC("SwapChainTarget")));
        }



        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;   // レンダーターゲットビュー
        heapDesc.NodeMask = 0;                                // GPUが1つの時は0、複数の時は識別用のbitを指定
        heapDesc.NumDescriptors = 2;                                // ディスクリプタの数。表と裏バッファの２つ。
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;  // ビューの情報をシェーダから参照する必要があるか

        auto result = rDevice.getNativeDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_rtvHeaps.ReleaseAndGetAddressOf()));

        OB_ENSURE(SUCCEEDED(result));


        DXGI_SWAP_CHAIN_DESC swcDesc = {};
        result = m_swapchain->GetDesc(&swcDesc);
        m_buffers.resize(swcDesc.BufferCount);
        D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();

        //SRGBレンダーターゲットビュー設定
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;


        for (u32 i = 0; i < swcDesc.BufferCount; ++i) {
            result = m_swapchain->GetBuffer(i, IID_PPV_ARGS(m_buffers[i].ReleaseAndGetAddressOf()));
            ms_device->CreateRenderTargetView(m_backBuffers[i].Get(), &rtvDesc, handle);
            handle.ptr += ms_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }



        m_viewport = CD3DX12_VIEWPORT(m_backBuffers[0].Get());
        m_scissorrect = CD3DX12_RECT(0, 0, (U32)m_viewport.Width, (U32)m_viewport.Height);
    }

}// namespace ob::graphic::dx12