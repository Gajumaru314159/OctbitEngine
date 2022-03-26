//***********************************************************
//! @file
//! @brief		デバイス実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "DeviceImpl.h"
#include<Plugins/GraphicDirectX12/SwapChain/SwapChainImpl.h>
#include<Plugins/GraphicDirectX12/Texture/TextureImpl.h>
#include<Plugins/GraphicDirectX12/Texture/RenderTextureImpl.h>
#include<Plugins/GraphicDirectX12/Utility/Utility.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    DeviceImpl::DeviceImpl(FeatureLevel featureLevel)
        :m_featureLevel(featureLevel) {
        initialize();
    }


    void DeviceImpl::update(){
        // 
        
        // コマンドをGPUに送信
        
        // 描画結果を画面に反映
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ディスプレイを生成
    //@―---------------------------------------------------------------------------
    ISwapChain* DeviceImpl::createSwapChain(const SwapchainDesc& desc) {
        return new SwapChainImpl(*this, desc);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  テクスチャを生成
    //@―---------------------------------------------------------------------------
    ob::graphic::ITexture* DeviceImpl::createTexture(const TextureDesc& desc, StringView name) {
        return new TextureImpl(*this, desc, name);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  レンダーテクスチャを生成
    //@―---------------------------------------------------------------------------
    ob::graphic::IRenderTexture* DeviceImpl::createRenderTexture(const gsl::span<TextureDesc> targets, const TextureDesc& depth, StringView name) {
        return new RenderTextureImpl(*this, targets, depth, name);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  初期化
    //@―---------------------------------------------------------------------------
    void DeviceImpl::initialize() {

        // デバイスの初期化
        initializeDXGIDevice();

        // コマンドキューの初期化
        initializeCommand();

        //// スワップチェーンの作成
        //if (FAILED(CreateSwapChain())) {
        //	LOG_FATAL_EX("Graphic", "Failed CreateSwapChain()");
        //}
        //// 最終レンダリング先の作成
        //if (FAILED(CreateFinalRenderTargets())) {
        //	LOG_FATAL_EX("Graphic", "Failed CreateFinalRenderTargets()");
        //}
        ////フェンスの作成
        //if (FAILED(CreateFence())) {
        //	LOG_FATAL_EX("Graphic", "Failed CreateFence()");
        //}

    }


    //@―---------------------------------------------------------------------------
    //! @brief  DXGIDeviceの初期化
    //@―---------------------------------------------------------------------------
    void DeviceImpl::initializeDXGIDevice() {
        // ファクトリの生成
        UINT flagsDXGI = 0;
        flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;
        auto result = ::CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {
            LOG_FATAL_EX("Graphic","CreateDXGIFactoryに失敗 [{0}]", Utility::getErrorMessage(result).c_str());
        }

        // アダプターの列挙し、メモリ量が最大のグラフィックボードを選択
        std::vector<IDXGIAdapter*> adapters;

        IDXGIAdapter* tmpAdapter = nullptr;
        IDXGIAdapter* selectedAdapter = nullptr;
        SIZE_T maxVideoCardMemory = 0;
        for (int i = 0; m_dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
            DXGI_ADAPTER_DESC adesc = {};
            tmpAdapter->GetDesc(&adesc);

            SIZE_T videoCardMemory = (SIZE_T)(adesc.DedicatedVideoMemory / 1024 / 1024);  //(MB)

            if (maxVideoCardMemory < videoCardMemory) {
                selectedAdapter = tmpAdapter;
                maxVideoCardMemory = videoCardMemory;
            }
        }

        // 対応しているレベルで生成
        D3D_FEATURE_LEVEL featureLevel;
        D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        result = S_FALSE;
        for (auto level : levels) {
            if (SUCCEEDED(::D3D12CreateDevice(selectedAdapter, level, IID_PPV_ARGS(&m_device)))) {
                featureLevel = level;
                result = S_OK;
                break;
            }
        }
        if (FAILED(result)) {
            LOG_FATAL_EX("Graphic", "D3D12CreateDeviceに失敗 [{0}]", Utility::getErrorMessage(result).c_str());
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  コマンドの初期化
    //@―---------------------------------------------------------------------------
    void DeviceImpl::initializeCommand() {

        // コマンドアロケータを生成
        auto result = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocator.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {
            LOG_FATAL_EX("Graphic", "CreateCommandAllocatorに失敗 [{0}]", Utility::getErrorMessage(result).c_str());
        }
        
        // システム・コマンドリストを生成
        result = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(m_systemCmdList.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {
            LOG_FATAL_EX("Graphic", "CreateCommandListに失敗 [{0}]", Utility::getErrorMessage(result).c_str());
        }


        // コマンドキューの生成
        D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
        cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;				// コマンドリストと合わせる
        cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;				// タイムアウトなし
        cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// コマンドキューの優先度
        cmdQueueDesc.NodeMask = 0;										// GPUが1つの時は0、複数の時は識別用のbitを指定

        result = m_device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf()));


    }


    //@―---------------------------------------------------------------------------
    //! @brief  ビデオカード情報を初期化
    //@―---------------------------------------------------------------------------
    void DeviceImpl::initializeVideoCardInfo() {

    }

}// namespace ob::graphic::dx12