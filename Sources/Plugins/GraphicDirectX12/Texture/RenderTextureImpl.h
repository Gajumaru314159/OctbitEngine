//***********************************************************
//! @file
//! @brief		レンダーテクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IRenderTexture.h>
#include <Framework/Graphic/Types/TextureDesc.h>


namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      レンダーテクスチャ実装(DirectX12)
    //! 
    //! @details    描画可能なテクスチャ。
    //@―---------------------------------------------------------------------------
    class RenderTextureImpl :public IRenderTexture {
    public:

        RenderTextureImpl(ID3D12Device& rDevice, const vector<TextureDesc>& desc, DepthStencilFormat dsFormat,StringView name);

    public:


    private:

        vector<ComPtr<ID3D12Resource>> m_resources;             //!< リソースリスト
        ComPtr<ID3D12Resource> m_depth;                         //!< デプステクスチャ

        ComPtr<ID3D12DescriptorHeap> m_srvHeap;                 //!< シェーダリソースビュー
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;                 //!< レンダー・ターゲット・ビュー
        ComPtr<ID3D12DescriptorHeap> m_dsvHeap;                 //!< デプス・ステンシル・ビュー

        ComPtr<ID3D12GraphicsCommandList> m_cmdList;            //!< コマンドリスト
        ComPtr<ID3D12CommandAllocator> m_cmdAllocator;          //!< コマンド・アロケータ

        D3D12_VIEWPORT m_viewport;         //ビューポート
        D3D12_RECT m_scissorrect;          //シザー矩形

    };

}// namespace ob::graphic::dx12