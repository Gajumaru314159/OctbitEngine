//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/Core/Misc/BlobView.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>



//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi {
    class DirectX12Device;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi {

    class DirectX12Texture :public rhi::RenderTexture {
    public:

        //! @brief      空テクスチャを生成
        DirectX12Texture(DirectX12Device& device, const TextureDesc& desc);

        //! @brief      色データから新しいテクスチャを作成
        DirectX12Texture(DirectX12Device& device, StringView name, TextureType type, Size size,Span<const IntColor> colors);

        //! @brief      テクスチャバイナリから生成
        DirectX12Texture(DirectX12Device& device, StringView name, BlobView blob);

        //! @brief      ベースのテクスチャを指定して異なるビューを持つテクスチャを作成
        DirectX12Texture(DirectX12Device& device, const TextureViewDesc& desc);

        //! @brief      妥当な状態か
        bool isValid()const;

        //! @brief      名前を取得
        const String& getName()const override;

        //! @brief      名前を設定
        void setName(StringView)override;

        //! @brief      定義取得
        const TextureDesc& desc()const override;

        //! @brief      BindlessHandleを取得
        BindlessHandle handle()const override;

        //! @brief      定義取得
        const RenderTextureDesc& descOfRenderTexture()const override;

    public:

        //! @brief      RenderTextureDesc からRenderTextureを生成
        DirectX12Texture(DirectX12Device& device, const RenderTextureDesc& desc);

        //! @brief      SwapChainのリソースからRenderTextureを生成
        DirectX12Texture(DirectX12Device& device, const ComPtr<ID3D12Resource>& resource,D3D12_RESOURCE_STATES state,StringView name);

    public:

        //! @brief      リソースを取得
        ID3D12Resource* getResource()const;

        //! @brief      ネイティブリソースを解放
        //! @details    SwapChainのリサイズ用
        void releaseNative() { m_resource = nullptr; }

        //! @brief      RTVを取得
        const DescriptorHandle& getRTV()const { return m_hRTV; }

        //! @brief      DSVを取得
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
        void createUAV(D3D12_CPU_DESCRIPTOR_HANDLE handle,s32 slice)const;

        //! @brief      遷移バリアを追加
        bool addResourceTransition(D3D12_RESOURCE_BARRIER& barrier, D3D12_RESOURCE_STATES state,s32 subresource=-1);

    private:

        //! @brief      共通初期化処理
        void initialize();

    protected:

        class DirectX12Device&     m_device;

        TextureDesc             m_desc;         //!< 定義
        TextureViewDesc         m_viewDesc;     //!< ビュー定義
		DescriptorHandle        m_handle;       //!< デスクリプタハンドル
        ComPtr<ID3D12Resource>  m_resource;     //!< リソース 


        // TODO RenderTextureのみ必要なメンバはUPtrで囲ってTexture生成時にはメモリを消費しないようにする
        RenderTextureDesc       m_renderDesc;   //!< 定義
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
namespace ob::rhi {

    //! @brief  妥当な状態か
    inline bool DirectX12Texture::isValid()const {
        return !!m_resource;
    }

    //! @brief      名前を取得
    inline const String& DirectX12Texture::getName()const {
        return m_desc.name;
    }

    //! @brief      名前を設定
    inline void DirectX12Texture::setName(StringView name) {
        Utility::SetName(m_resource.Get(), name);
    }

    //! @brief      定義取得
    inline const TextureDesc& DirectX12Texture::desc()const {
        return m_desc;
    }

    //! @brief      BindlessHandleを取得
    inline BindlessHandle DirectX12Texture::handle()const {
		BindlessHandle handle;
		handle.type = BindingType::Texture;
		handle.index = m_handle.getBindlessIndex();
        return handle;
    }

    //! @brief      定義取得
    //! @note		RenderTexutreとして使用される場合のみアクセス可能
    inline const RenderTextureDesc& DirectX12Texture::descOfRenderTexture()const {
        return m_renderDesc;
    }

    //! @brief      リソースを取得
    inline ID3D12Resource* DirectX12Texture::getResource() const {
        return m_resource.Get();
    }

}
