//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "RenderTextureImpl.h"
#include <Plugins/GraphicDirectX12/Utility/Utility.h>

namespace ob::graphic::dx12 {

    RenderTextureImpl::RenderTextureImpl(ID3D12Device& rDevice, const vector<TextureDesc>& descs, DepthStencilFormat dsFormat, StringView name) {
        const s32 targetNum = get_size(descs);
        OB_REQUIRE_EX(0 < targetNum && targetNum <= 8, "マルチターゲットの数が不正です。1以上8以下にしてください。[{}]", targetNum);

        Size size = descs[0].size;

        vector<ComPtr<ID3D12Resource>> resources(targetNum);

        HRESULT result;

        // RTVディスクリプタの生成
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heapDesc.NodeMask = 0;
        heapDesc.NumDescriptors = targetNum;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        result = rDevice.CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_rtvHeap.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {

            return;
        }

        auto heapHandleRTV = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

        // SRVディスクリプタの生成
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        result = rDevice.CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_srvHeap.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {
            return;
        }
        auto heapHandleSRV = m_srvHeap->GetCPUDescriptorHandleForHeapStart();




        const auto incSizeRTV = rDevice.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        const auto incSizeSRV = rDevice.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        for (s32 i = 0; i < targetNum; i++) {
            auto& desc = descs[i];
            auto format = Utility::convertTextureFormat(descs[i].format);

            // メインリソースを生成
            {
                auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
                auto resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, size.width, size.height);
                resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

                float clearColor[4] = { 0.0,0.0,0.0,1.0 };
                auto clearValue = CD3DX12_CLEAR_VALUE(format, clearColor);

                result = rDevice.CreateCommittedResource(
                    &texHeapProp,
                    D3D12_HEAP_FLAG_NONE,
                    &resourceDesc,
                    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                    &clearValue,
                    IID_PPV_ARGS(resources[i].ReleaseAndGetAddressOf())
                );
                if (FAILED(result)) {
                    return;
                }
            }

            // レンダーターゲットビューを生成
            {
                D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
                rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
                rtvDesc.Format = format;

                rDevice.CreateRenderTargetView(resources[i].Get(), &rtvDesc, heapHandleRTV);
                heapHandleRTV.ptr += incSizeRTV;
            }

            // シェーダリソースビューを生成
            {
                D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Format = format;
                srvDesc.Texture2D.MipLevels = 1;
                srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

                rDevice.CreateShaderResourceView(resources[i].Get(), &srvDesc, heapHandleSRV);
                heapHandleSRV.ptr += incSizeSRV;
            }

        }


        // デプスリソースを生成
        if (dsFormat!=DepthStencilFormat::Unused) {
            auto format = Utility::convertTextureFormat(dsFormat);

            auto depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, size.width, size.height);
            depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            auto depthHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

            D3D12_CLEAR_VALUE depthClearValue = CD3DX12_CLEAR_VALUE(format, 1.0f, 0);
            result = rDevice.CreateCommittedResource(
                &depthHeapProp,
                D3D12_HEAP_FLAG_NONE,
                &depthResDesc,
                D3D12_RESOURCE_STATE_DEPTH_WRITE,
                &depthClearValue,
                IID_PPV_ARGS(m_depth.ReleaseAndGetAddressOf()));

            if (FAILED(result)) {
                return;
            }


            //深度のためのデスクリプタヒープ作成
            D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
            dsvHeapDesc.NumDescriptors = 1;
            dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

            result = rDevice.CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(m_dsvHeap.ReleaseAndGetAddressOf()));
            if (FAILED(result)) {
                return;
            }

            //深度ビュー作成
            D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
            dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//デプス値に32bit使用
            dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
            rDevice.CreateDepthStencilView(m_depth.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

        }


        result = rDevice.CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cmdAllocator.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {
            return;
        }
        result = rDevice.CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(m_cmdList.ReleaseAndGetAddressOf()));
        if (FAILED(result)) {
            return;
        }


        m_resources = resources;

        m_viewport = CD3DX12_VIEWPORT(m_resources[0].Get());
        m_scissorrect = CD3DX12_RECT(0, 0, (LONG)m_viewport.Width, (LONG)m_viewport.Height);
    }

}// namespace ob::graphic::dx12