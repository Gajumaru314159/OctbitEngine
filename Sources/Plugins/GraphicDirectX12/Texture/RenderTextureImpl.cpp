//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "RenderTextureImpl.h"
#include <Plugins/GraphicDirectX12/Utility/Utility.h>
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/Texture/TextureImpl.h>

#include <Framework/Graphic/Private/Device.h>
#include <Framework/Foundation/String/StringEncoder.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //@―---------------------------------------------------------------------------
    RenderTextureImpl::RenderTextureImpl(DeviceImpl& rDevice, const gsl::span<TextureDesc> targets, const TextureDesc& depth, StringView name)
        :IRenderTexture(name) {
        
        HRESULT result;
        const s32 targetNum = get_size(targets);
        OB_CHECK_ASSERT(0 < targetNum && targetNum <= 8, "マルチターゲットの数が不正です。1以上8以下にしてください。[{}]", targetNum);

        auto& nativeDevice = rDevice.getNativeDevice();


        // RTVディスクリプタを生成
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heapDesc.NodeMask = 0;
        heapDesc.NumDescriptors = targetNum;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        result = nativeDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_rtvHeap.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {
            LOG_FATAL_EX("Graphic", "ID3D12Device::CreateDescriptorHeapに失敗 [{0}]", Utility::getErrorMessage(result).c_str());
        }

        // SRVディスクリプタを生成
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        result = nativeDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_srvHeap.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {
            LOG_FATAL_EX("Graphic", "ID3D12Device::CreateDescriptorHeapに失敗 [{0}]", Utility::getErrorMessage(result).c_str());
        }


        // ターゲットテクスチャ生成
        auto heapHandleRTV = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
        auto heapHandleSRV = m_srvHeap->GetCPUDescriptorHandleForHeapStart();

        const auto incSizeRTV = nativeDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        const auto incSizeSRV = nativeDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        // レンダーターゲットを生成
        s32 index = 0;
        for (auto& desc:targets) {

            auto format = Utility::convertTextureFormat(desc.format);

            auto subName = fmt::format(TC("{0}_Color{1}"), getName(), index);

            // メインリソースを生成
            auto& texture = m_textures.emplace_back(Texture(desc, subName));
            auto pTexture = Device::GetImpl<TextureImpl>(texture);
            if (pTexture==nullptr) {
                LOG_WARNING_EX("Graphic","RenderTextureのレンダーターゲットの生成に失敗");
                return;
            }

            // レンダーターゲットビューを生成
            {
                D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
                rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
                rtvDesc.Format = format;

                nativeDevice->CreateRenderTargetView(pTexture->getResource(), &rtvDesc, heapHandleRTV);
                heapHandleRTV.ptr += incSizeRTV;
            }

            // シェーダリソースビューを生成
            {
                D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Format = format;
                srvDesc.Texture2D.MipLevels = 1;
                srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

                nativeDevice->CreateShaderResourceView(pTexture->getResource(), &srvDesc, heapHandleSRV);
                heapHandleSRV.ptr += incSizeSRV;
            }

            ++index;

        }

        // デプス・ステンシル生成
        {
            auto format = Utility::convertTextureFormat(depth.format);

            // メインリソースを生成
            auto subName = fmt::format(TC("{0}_Depth"), getName());
            m_depth = Texture(depth, subName);
            auto pTexture = Device::GetImpl<TextureImpl>(m_depth);
            if (pTexture == nullptr) {
                LOG_WARNING_EX("Graphic", "RenderTextureのレンダーターゲットの生成に失敗");
                return;
            }


            //深度ビューのデスクリプタヒープ作成
            D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
            dsvHeapDesc.NumDescriptors = 1;
            dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

            result = nativeDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(m_dsvHeap.ReleaseAndGetAddressOf()));
            if (FAILED(result)) {
                LOG_FATAL_EX("Graphic", "ID3D12Device::CreateDescriptorHeapに失敗 [{0}]", Utility::getErrorMessage(result).c_str());
            }

            //深度ビュー作成
            D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
            dsvDesc.Format = pTexture->getResource()->GetDesc().Format;//デプス値に32bit使用
            dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
            nativeDevice->CreateDepthStencilView(pTexture->getResource(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
            if (FAILED(result)) {
                LOG_FATAL_EX("Graphic", "ID3D12Device::CreateDepthStencilViewに失敗 [{0}]", Utility::getErrorMessage(result).c_str());
            }

        }

        // コマンド・アロケータ生成
        result = nativeDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cmdAllocator.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {
            return;
        }
        // コマンド・リスト生成
        result = nativeDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {
            return;
        }

        // 0番目のターゲットをビューポートサイズとする
        m_viewport = CD3DX12_VIEWPORT(reinterpret_cast<TextureImpl*>(Device::GetImpl(m_textures[0]))->getResource());
        m_scissorrect = CD3DX12_RECT(0, 0, (LONG)m_viewport.Width, (LONG)m_viewport.Height);

        m_initialized = true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    RenderTextureImpl::~RenderTextureImpl() {
    }


    //@―---------------------------------------------------------------------------
    //! @brief  妥当な状態か
    //@―---------------------------------------------------------------------------
    bool RenderTextureImpl::isValid()const {
        return m_initialized;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  テクスチャを取得
    //! 
    //! @param index    マルチレンダーターゲットのインデックス
    //@―---------------------------------------------------------------------------
    const Texture& RenderTextureImpl::getTexture(s32 index)const {
        if (is_in_range(index, m_textures)) {
            return m_textures[index];
        }
        return Texture();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デプス・テクスチャを取得
    //@―---------------------------------------------------------------------------
    const Texture& RenderTextureImpl::getDepthStencilTexture()const {
        return m_depth;
    }

}// namespace ob::graphic::dx12