//***********************************************************
//! @file
//! @brief		テクスチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/Core/Misc/BlobView.h>

namespace ob::rhi::vulkan {

    class VulkanRHI;

    class TextureImpl :public rhi::RenderTexture {
    public:

        //! @brief      TextureDesc から空のテクスチャを生成
        TextureImpl(VulkanRHI& rhi, const TextureDesc& desc);

        //! @brief      IntColorの配列 から空のテクスチャを生成
        TextureImpl(VulkanRHI& rhi, StringView name, TextureType type, Size size,Span<const IntColor> colors);

        //! @brief      テクスチャバイナリから生成
        TextureImpl(VulkanRHI& rhi, StringView name,BlobView blob);

        //! @brief      デストラクタ
        ~TextureImpl();

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
        TextureImpl(VulkanRHI& rhi, const RenderTextureDesc& desc);

        //! @brief      SwapChainのリソースからRenderTextureを生成
        TextureImpl(VulkanRHI& rhi, VkImage image, vk::Format format,StringView name);

    public:

    private:

		VulkanRHI&              m_rhi;

        TextureDesc             m_desc;         //!< 定義
        RenderTextureDesc       m_renderDesc;   //!< 定義

        vk::raii::Image         m_image = nullptr;
		vk::raii::DeviceMemory	m_memory = nullptr;

        //ComPtr<ID3D12Resource>  m_resource;     //!< リソース        
        //
        //// TODO RenderTextureのみ必要なメンバはUPtrで囲ってTexture生成時にはメモリを消費しないようにする
        vk::raii::ImageView       m_hRTV = nullptr;
        vk::raii::ImageView       m_hDSV = nullptr;

        //
        //D3D12_VIEWPORT          m_viewport{};   //!< ビューポート
        //D3D12_RECT              m_scissorRect{};//!< シザー矩形
        //
        //D3D12_RESOURCE_STATES   m_state = D3D12_RESOURCE_STATE_COMMON;

    };

}



//===============================================================
// インライン
//===============================================================
namespace ob::rhi::vulkan {

    //! @brief      名前を取得
    inline const String& TextureImpl::getName()const {
        return m_desc.name;
    }

    //! @brief      名前を設定
    inline void TextureImpl::setName(StringView name) {
        //Utility::SetName(m_resource.Get(), name);
    }

    //! @brief      定義取得
    inline const TextureDesc& TextureImpl::desc()const {
        return m_desc;
    }

    //! @brief      定義取得
    //! @note		RenderTexutreとして使用される場合のみアクセス可能
    inline const RenderTextureDesc& TextureImpl::descOfRenderTexture()const {
        return m_renderDesc;
    }

}
