//***********************************************************
//! @file
//! @brief		レンダーテクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IRenderTexture.h>
#include <Framework/Graphic/Types/TextureDesc.h>
#include <Framework/Graphic/Texture.h>


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
        RenderTextureImpl(DeviceImpl& rDevice, const gsl::span<TextureDesc> targets, const TextureDesc& depth);


        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        ~RenderTextureImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ取得
        //@―---------------------------------------------------------------------------
        const Texture getTexture(s32 index)const override;
        const Texture getDepthStencilTexture()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      描画ターゲット枚数を取得
        //! 
        //! @details    デプス・ステンシルテクスチャは含まれません。
        //@―---------------------------------------------------------------------------
        s32 getTargetNum()const override;


    private:

        vector<Texture> m_textures;                     //!< ターゲット・テクスチャ・リスト
        Texture m_depth;                                //!< デプス・ステンシル・テクスチャ

        ComPtr<ID3D12DescriptorHeap> m_srvHeap;         //!< シェーダリソースビュー
        ComPtr<ID3D12DescriptorHeap> m_rtvHeap;         //!< レンダー・ターゲット・ビュー
        ComPtr<ID3D12DescriptorHeap> m_dsvHeap;         //!< デプス・ステンシル・ビュー

        ComPtr<ID3D12GraphicsCommandList> m_cmdList;    //!< コマンドリスト
        ComPtr<ID3D12CommandAllocator> m_cmdAllocator;  //!< コマンド・アロケータ

        D3D12_VIEWPORT m_viewport;                      //!< ビューポート
        D3D12_RECT m_scissorrect;                       //!< シザー矩形

        bool m_initialized =false;
    };

}// namespace ob::graphic::dx12