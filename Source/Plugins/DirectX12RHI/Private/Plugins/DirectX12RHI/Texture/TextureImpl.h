//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/Core/Misc/BlobView.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>



//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi::dx12 {
    class DeviceImpl;
    class ResourceStateCache;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::dx12 {

    class TextureImpl :public rhi::RenderTexture {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      TextureDesc から空のテクスチャを生成
        //@―---------------------------------------------------------------------------
        TextureImpl(DeviceImpl& rDevice, const TextureDesc& desc);

        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャバイナリから生成
        //@―---------------------------------------------------------------------------
        TextureImpl(DeviceImpl& rDevice, BlobView blob);


        //@―---------------------------------------------------------------------------
        //! @brief      妥当な状態か
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      サイズ
        //@―---------------------------------------------------------------------------
        Size size()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ・フォーマットを取得
        //@―---------------------------------------------------------------------------
        TextureFormat format()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      ミップレベルを取得
        //@―---------------------------------------------------------------------------
        s32 mipLevels()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      定義を取得
        //@―---------------------------------------------------------------------------
        const TextureDesc& getDesc() const;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      RenderTextureDesc からRenderTextureを生成
        //@―---------------------------------------------------------------------------
        TextureImpl(DeviceImpl& rDevice, const RenderTextureDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief      SwapChainのリソースからRenderTextureを生成
        //@―---------------------------------------------------------------------------
        TextureImpl(DeviceImpl& rDevice, const ComPtr<ID3D12Resource>& resource,D3D12_RESOURCE_STATES state);


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      リソースを取得
        //@―---------------------------------------------------------------------------
        ID3D12Resource* getResource()const;


        //@―---------------------------------------------------------------------------
        //! @brief      Viewを取得
        //@―---------------------------------------------------------------------------
        const DescriptorHandle& getSRV()const { return m_hSRV; }
        const DescriptorHandle& getRTV()const { return m_hRTV; }
        const DescriptorHandle& getDSV()const { return m_hDSV; }

        const D3D12_VIEWPORT& getViewport()const { return m_viewport; }
        const D3D12_RECT& getScissorRect()const { return m_scissorRect; }


        void clear(ID3D12GraphicsCommandList* cmdList);

        //@―---------------------------------------------------------------------------
        //! @brief      シェーダリソースビューを生成
        //@―---------------------------------------------------------------------------
        void createSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const;

        //@―---------------------------------------------------------------------------
        //! @brief      遷移バリアを追加
        //@―---------------------------------------------------------------------------
        bool addResourceTransition(D3D12_RESOURCE_BARRIER& barrier, D3D12_RESOURCE_STATES state,s32 subresource=-1);


    private:

        void onNameChanged()override;


    protected:

        class DeviceImpl& m_device;

        TextureDesc             m_desc;         //!< 定義
        RenderTextureDesc       m_renderDesc;   //!< 定義

        ComPtr<ID3D12Resource>  m_resource;     //!< リソース
        DescriptorHandle        m_hSRV;         //!< デスクリプタハンドル
        DescriptorHandle        m_hRTV;         //!< デスクリプタハンドル
        DescriptorHandle        m_hDSV;         //!< デスクリプタハンドル

        D3D12_VIEWPORT          m_viewport{};   //!< ビューポート
        D3D12_RECT              m_scissorRect{};//!< シザー矩形

        D3D12_RESOURCE_STATES   m_state = D3D12_RESOURCE_STATE_COMMON;

    };

}// namespace ob::rhi::dx12



//===============================================================
// インライン
//===============================================================
namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief      リソースを取得
    //@―---------------------------------------------------------------------------
    inline ID3D12Resource* TextureImpl::getResource() const {
        return m_resource.Get();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      定義を取得
    //@―---------------------------------------------------------------------------
    inline const TextureDesc& TextureImpl::getDesc() const {
        return m_desc;
    }

}// namespace ob::rhi::dx12
