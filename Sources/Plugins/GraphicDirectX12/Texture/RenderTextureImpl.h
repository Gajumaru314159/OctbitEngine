//***********************************************************
//! @file
//! @brief		レンダーテクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IRenderTexture.h>
#include <Framework/Graphic/Types/TextureDesc.h>


//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic::dx12 {
    class DeviceImpl;
    class TextureImpl;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      レンダーテクスチャ実装(DirectX12)
    //! 
    //! @details    描画可能なテクスチャ。
    //@―---------------------------------------------------------------------------
    class RenderTextureImpl :public graphic::IRenderTexture {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        RenderTextureImpl(DeviceImpl& rDevice, const gsl::span<TextureDesc> targets, const TextureDesc& depth,StringView name);

    public:

        graphic::ITexture* getTexture(s32 index)const override;
        graphic::ITexture* getDepthStencilTexture()const override;


    private:

        vector<std::unique_ptr<TextureImpl>> m_textures;        //!< ターゲット・テクスチャ・リスト
        std::unique_ptr<TextureImpl> m_depth;                   //!< デプス・ステンシル・テクスチャ

        ComPtr<ID3D12DescriptorHeap> m_srvHeap;                 //!< シェーダリソースビュー
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;                 //!< レンダー・ターゲット・ビュー
        ComPtr<ID3D12DescriptorHeap> m_dsvHeap;                 //!< デプス・ステンシル・ビュー

        ComPtr<ID3D12GraphicsCommandList> m_cmdList;            //!< コマンドリスト
        ComPtr<ID3D12CommandAllocator> m_cmdAllocator;          //!< コマンド・アロケータ

        D3D12_VIEWPORT m_viewport;         //ビューポート
        D3D12_RECT m_scissorrect;          //シザー矩形

    };

}// namespace ob::graphic::dx12