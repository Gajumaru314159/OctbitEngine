//***********************************************************
//! @file
//! @brief		レンダーテクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IRenderTexture.h>
#include <Framework/Graphic/Types/TextureDesc.h>
/*

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      レンダーテクスチャ実装(DirectX12)
    //! 
    //! @details    描画可能なテクスチャ。
    //@―---------------------------------------------------------------------------
    class RenderTextureImpl :public IRenderTexture {
    public:

        RenderTextureImpl(ID3D12Device* pDevice, const vector<TextureDesc>& desc,StringView name);

    private:

        vector<ComPtr<ID3D12Resource>> m_resources;
        ComPtr<ID3D12DescriptorHeap> m_srvHeap;

        ComPtr<ID3D12Resource> m_depth = nullptr;               // デプステクスチャ
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap = nullptr;       // レンダー・ターゲット・ビュー
        ComPtr<ID3D12DescriptorHeap> m_dsvHeap = nullptr;       // デプス・ステンシル・ビュー
        ComPtr<ID3D12GraphicsCommandList> m_cmdList = nullptr;  // コマンドリスト
        ComPtr<ID3D12CommandAllocator> m_cmdAllocator = nullptr;// コマンド・アロケータ

        D3D12_VIEWPORT m_viewport;         //ビューポート
        D3D12_RECT m_scissorrect;          //シザー矩形

    };

}// namespace ob::graphic::dx12

*/