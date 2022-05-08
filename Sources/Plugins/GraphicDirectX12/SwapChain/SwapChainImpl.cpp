//***********************************************************
//! @file
//! @brief		スワップ・チェーン実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "SwapChainImpl.h"
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/Utility/Utility.h>
#include <Plugins/GraphicDirectX12/Utility/TypeConverter.h>
#include <Framework/Graphic/Interface/ITexture.h>
#include <Framework/Platform/Window/WindowNativaAccessor.h>

namespace {
    int static const s_maxSwapChainCount = 4;
}

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    SwapChainImpl::SwapChainImpl(DeviceImpl& rDevice, const SwapchainDesc& desc)
        : m_desc(desc)
    {

        if (desc.window == nullptr) {
            LOG_ERROR_EX("Graphic","SwapchainDesc::windowがnullptrです。");
            return;
        }

        // サイズが指定されていない場合はウィンドウサイズを使用
        if (m_desc.size.width == 0 || m_desc.size.height == 0)m_desc.size = m_desc.window->getSize();

        m_displayViewFormat = desc.hdr ? TextureFormat::R10G10B10A2 : TextureFormat::RGBA8;
        m_nativeDisplayViewFormat = TypeConverter::convert(m_displayViewFormat);
        m_nativeSwapChainFormat = TypeConverter::convert(m_displayViewFormat);
        m_syncInterval = desc.vsync ? 1 : 0;

        if (!createSwapChain(rDevice))return;
        if (!createBuffer(rDevice))return;

        m_initialized = true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    SwapChainImpl::~SwapChainImpl() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief  妥当なオブジェクトか
    //@―---------------------------------------------------------------------------
    bool SwapChainImpl::isValid()const {
        return m_initialized;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  定義を取得
    //@―---------------------------------------------------------------------------
    const SwapchainDesc& SwapChainImpl::getDesc()const noexcept {
        return m_desc;
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
    void SwapChainImpl::update(const graphic::Texture& texture) {

        if (!texture) {
            LOG_FATAL_EX("Graphic", "スワップチェーンの更新に失敗。空テクスチャを設定できません。")
        }


        auto result = m_swapchain->Present(m_syncInterval, 0);

        if (FAILED(result)) {
            Utility::outputFatalLog(result, TC("IDXGUISwapChain::Present()"));
            LOG_FATAL_EX("Graphic","スワップチェーンの更新に失敗")
            return;
        }

        m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();

    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    bool SwapChainImpl::createSwapChain(DeviceImpl& rDevice) {
        auto& window = m_desc.window;

        UINT sampleQuarity = 0;
        UINT sampleCount = 1;
        HWND hWnd = static_cast<HWND>(ob::platform::WindowNativeAccessor::getHWND(*window));
        {
            D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS feature;
            auto result = rDevice.getNativeDevice()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &feature, sizeof(feature));
            if (SUCCEEDED(result)) {
                LOG_INFO_EX("Graphic", "最大マルチサンプルカウント={}", feature.SampleCount);
                LOG_INFO_EX("Graphic", "最大マルチサンプルクオリティ={}", feature.NumQualityLevels);
                //sampleQuarity = feature.SampleCount;
                //sampleCount = feature.NumQualityLevels;
            }
        }

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc.Width = m_desc.size.width;                                 // 画面解像度【横】
        swapChainDesc.BufferDesc.Height = m_desc.size.height;                               // 画面解像度【縦】
        swapChainDesc.BufferDesc.Format = TypeConverter::convert(m_desc.format);     // ピクセルフォーマット
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60000;                             // リフレッシュ・レート分子
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1000;                            // リフレッシュ・レート分母
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // スキャンラインの順番 => 指定なし
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;                     //解像度に合うように同補正するか => 拡大

        swapChainDesc.SampleDesc.Quality = sampleQuarity;                                   // マルチサンプル・クオリティ
        swapChainDesc.SampleDesc.Count = sampleCount;                                       // マルチサンプル・カウント

        swapChainDesc.BufferCount = m_desc.bufferCount;						            // バッファの数
        swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;						            // バックバッファとして使用
        swapChainDesc.OutputWindow = hWnd;                                                  // ウィンドウ
        swapChainDesc.Windowed = TRUE;                                                      // ※公式リファレンスによるとフルスクリーン指定は別ので行う
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;                           // Present後破棄

        swapChainDesc.Flags =
            //DXGI_SWAP_CHAIN_FLAG_NONPREROTATED |                  // フルスクリーン時自動回転
            DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH |                // ResizeTargetでサイズ変更許可
            //DXGI_SWAP_CHAIN_FLAG_DISPLAY_ONLY |                   // リモートアクセス禁止
            //DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT |  // フルスクリーン以外で描画待機
            //DXGI_SWAP_CHAIN_FLAG_FULLSCREEN_VIDEO |               // フルスクリーンビデオ
            //DXGI_SWAP_CHAIN_FLAG_YUV_VIDEO |                      // YUVビデオのスワップチェーン
            0;

        // スワップチェイン生成
        auto result = rDevice.getFactory()->CreateSwapChain(
            rDevice.getCommandQueue().Get(),
            &swapChainDesc,
            (IDXGISwapChain**)m_swapchain.ReleaseAndGetAddressOf());

        if (FAILED(result)) {
            Utility::outputFatalLog(result, TC("IDXGIFactory::CreateSwapChain()"));
            return false;
        }


        if (window->isMainWindow()) {

            // Alt + Enter でウィンドウモードに変わらないようにする 
            rDevice.getFactory()->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

            if (window->getMode() == platform::WindowMode::FullScreen) {
                // TODO フルスクリーンの場合バックバッファをリサイズ
            }
        }

        m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();

        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      レンダーテクスチャを初期化
    //@―---------------------------------------------------------------------------
    bool SwapChainImpl::createBuffer(DeviceImpl& rDevice) {

        if (!is_in_range(m_desc.bufferCount, 1, s_maxSwapChainCount)) {
            LOG_ERROR_EX("Graphic","バックバッファの枚数が不正です。[Min=1,Max={0},Value={1}]", s_maxSwapChainCount, m_desc.bufferCount);
            return false;
        }

        // スワップチェインのRTVデスクリプタを生成
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;     // レンダーターゲットビュー
        heapDesc.NodeMask = 0;                              // GPUが1つの時は0、複数の時は識別用のbitを指定
        heapDesc.NumDescriptors = m_desc.bufferCount;     // ディスクリプタの数。表と裏バッファの２つ。
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;   // ビューの情報をシェーダから参照する必要があるか

        auto result = rDevice.getNativeDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_rtvHeaps.ReleaseAndGetAddressOf()));

        if (FAILED(result)) {
            // 枚数チェックをしているので呼ばれないはず
            Utility::outputFatalLog(result, TC("ID3D12Device::CreateDescriptorHeap()"));
            return false;
        }

        // バッファを生成
        m_buffers.resize(m_desc.bufferCount);
        D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvHeaps->GetCPUDescriptorHandleForHeapStart();

        // レンダーターゲットビュー生成
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = m_nativeSwapChainFormat;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

        for (s32 i = 0; i < m_desc.bufferCount; ++i) {
            auto& buffer = m_buffers[i];
            result = m_swapchain->GetBuffer(i, IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf()));
            if (FAILED(result)) {
                // 生成が正しければ呼ばれないはず
                Utility::outputFatalLog(result, TC("IDXGISwapChain::GetBuffer()"));
                return false;
            }
            rDevice.getNativeDevice()->CreateRenderTargetView(buffer.Get(), &rtvDesc, handle);
            handle.ptr += rDevice.getNativeDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        }

        m_viewport = CD3DX12_VIEWPORT(m_buffers[0].Get());
        m_scissorrect = CD3DX12_RECT(0, 0, (UINT)m_viewport.Width, (UINT)m_viewport.Height);
        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      カラースペースを設定
    //@―---------------------------------------------------------------------------
    bool SwapChainImpl::setColorSpace() {
        bool isHdrEnabled = m_desc.hdr;
        if (!isHdrEnabled)return false;

        // TODO Rec2020以外の指定対応
        DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
        UINT colorSpaceSupport;

        auto result = m_swapchain->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport);
        if (FAILED(result)) {
            Utility::outputFatalLog(result, TC("IDXGISwapChain::CheckColorSpaceSupport()"));
            return false;
        }

        if (colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT) {
            result = m_swapchain->SetColorSpace1(colorSpace);
            if (FAILED(result)) {
                Utility::outputFatalLog(result, TC("IDXGISwapChain::SetColorSpace1()"));
                return false;
            }
        }
        return true;
    }


}// namespace ob::graphic::dx12