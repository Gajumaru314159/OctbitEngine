//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/RHI/Display.h>
#include <Framework/Core/Misc/BlobView.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>



//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi::dx12 {
    class DirectX12RHI;
    class ResourceStateCache;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::dx12 {

    class TextureImpl :public rhi::RenderTexture {
    public:

        //! @brief      TextureDesc から空のテクスチャを生成
        TextureImpl(DirectX12RHI& rDevice, const TextureDesc& desc);

        //! @brief      IntColorの配列 から空のテクスチャを生成
        TextureImpl(DirectX12RHI& rDevice, StringView name, Size size,Span<const IntColor> colors);

        //! @brief      テクスチャバイナリから生成
        TextureImpl(DirectX12RHI& rDevice, StringView name,BlobView blob);


        //! @brief      妥当な状態か
        bool isValid()const override;


        //! @brief      名前を取得
        const String& getName()const override;


        //! @brief      名前を設定
        void setName(StringView)override;


        //! @brief      定義取得
        const TextureDesc& desc()const override;

        //! @brief      定義取得
        const RenderTextureDesc& descOfRenderTexture()const override;

    public:

        //! @brief      RenderTextureDesc からRenderTextureを生成
        TextureImpl(DirectX12RHI& rDevice, const RenderTextureDesc& desc);


        //! @brief      SwapChainのリソースからRenderTextureを生成
        TextureImpl(DirectX12RHI& rDevice, const ComPtr<ID3D12Resource>& resource,D3D12_RESOURCE_STATES state,StringView name);


    public:

        //! @brief      リソースを取得
        ID3D12Resource* getResource()const;


        //! @brief      ネイティブリソースを解放
        //! @details    Displayのリサイズ用
        void releaseNative() { m_resource = nullptr; }


        //! @brief      Viewを取得
        const DescriptorHandle& getRTV()const { return m_hRTV; }
        const DescriptorHandle& getDSV()const { return m_hDSV; }

        //! @brief      Viewportを取得
        const D3D12_VIEWPORT& getViewport()const { return m_viewport; }

        //! @brief      Rectを取得
        const D3D12_RECT& getScissorRect()const { return m_scissorRect; }

        //! @brief      クリアコマンドを記録
        void clear(ID3D12GraphicsCommandList* cmdList);

        //! @brief      SRVを生成
        void createSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const;

        //! @brief      UAVを生成
        private:
        void createUAV(D3D12_CPU_DESCRIPTOR_HANDLE handle,s32 slice)const;
        public:

        //! @brief      遷移バリアを追加
        bool addResourceTransition(D3D12_RESOURCE_BARRIER& barrier, D3D12_RESOURCE_STATES state,s32 subresource=-1);

    protected:

        class DirectX12RHI& m_device;

        TextureDesc             m_desc;         //!< 定義
        RenderTextureDesc       m_renderDesc;   //!< 定義

        ComPtr<ID3D12Resource>  m_resource;     //!< リソース

        // TODO RenderTextureのみ必要なメンバはUPtrで囲ってTexture生成時にはメモリを消費しないようにする
        DescriptorHandle        m_hRTV;         //!< デスクリプタハンドル
        DescriptorHandle        m_hDSV;         //!< デスクリプタハンドル

        D3D12_VIEWPORT          m_viewport{};   //!< ビューポート
        D3D12_RECT              m_scissorRect{};//!< シザー矩形

        D3D12_RESOURCE_STATES   m_state = D3D12_RESOURCE_STATE_COMMON;

    };

}



//===============================================================
// インライン
//===============================================================
namespace ob::rhi::dx12 {

    //! @brief      リソースを取得
    inline ID3D12Resource* TextureImpl::getResource() const {
        return m_resource.Get();
    }

}
