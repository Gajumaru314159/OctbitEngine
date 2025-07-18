//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/Core/Misc/BlobView.h>
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorHandle.h>

namespace ob::rhi {

    class VulkanRHI;

    class VulkanTexture :public rhi::RenderTexture {
    public:

        //! @brief      TextureDesc から空のテクスチャを生成
        VulkanTexture(VulkanRHI& rhi, const TextureDesc& desc);

        //! @brief      IntColorの配列 から空のテクスチャを生成
        VulkanTexture(VulkanRHI& rhi, StringView name, TextureType type, Size size,Span<const IntColor> colors);

        //! @brief      テクスチャバイナリから生成
        VulkanTexture(VulkanRHI& rhi, StringView name,BlobView blob);

        //! @brief      ベースのテクスチャを指定して異なるビューを持つテクスチャを作成
        VulkanTexture(VulkanRHI& rhi, const TextureViewDesc& desc);

        //! @brief      デストラクタ
        ~VulkanTexture();

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
        VulkanTexture(VulkanRHI& rhi, const RenderTextureDesc& desc);

        //! @brief      SwapChainのリソースからRenderTextureを生成
        VulkanTexture(VulkanRHI& rhi, VkImage image, vk::Format format, vk::Extent2D size, StringView name);

    public:

		//! @brief      テクスチャを取得
		vk::raii::Image& getNative() { return m_shared->image; }
		//! @brief      メモリを取得
		vk::raii::DeviceMemory& getMemory() { return m_shared->memory; }
        //! @brief      のイメージビューを取得(要修正)
        // vk::raii::ImageView& getSRV() { return m_hSRV; }
        //! @brief      レンダーテクスチャのイメージビューを取得
        vk::raii::ImageView& getRTV() { return m_hRTV; }
		//! @brief      レンダーテクスチャのデプスステンシルビューを取得
		vk::raii::ImageView& getDSV() { return m_hDSV; }

        void initialize();

        bool createView(vk::raii::ImageView& view);

    private:

		VulkanRHI&              m_rhi;

        TextureDesc             m_desc;         //!< 定義
		TextureViewDesc         m_viewDesc;     //!< ビュー定義
		VulkanDescriptorHandle  m_handle;       //!< デスクリプタハンドル

        struct SharedResource {
            vk::raii::DeviceMemory	memory = nullptr;
            vk::raii::Image         image = nullptr;
        };

        SPtr< SharedResource>   m_shared;
        vk::raii::ImageView     m_view = nullptr;

        // TODO RenderTextureのみ必要なメンバはUPtrで囲ってTexture生成時にはメモリを消費しないようにする
        RenderTextureDesc       m_renderDesc;   //!< 定義
        vk::raii::ImageView     m_hRTV = nullptr;
        vk::raii::ImageView     m_hDSV = nullptr;

    };

}



//===============================================================
// インライン
//===============================================================
namespace ob::rhi {

    //! @brief      名前を取得
    inline const String& VulkanTexture::getName()const {
        return m_desc.name;
    }

    //! @brief      名前を設定
    inline void VulkanTexture::setName(StringView name) {
        //Utility::SetName(m_resource.Get(), name);
    }

    //! @brief      定義取得
    inline const TextureDesc& VulkanTexture::desc()const {
        return m_desc;
    }

    //! @brief      BindlessHandleを取得
    inline BindlessHandle VulkanTexture::handle()const {
        BindlessHandle handle;
        handle.type = BindingType::Texture;
        handle.index = m_handle.getBindlessIndex();
        return handle;
    }

    //! @brief      定義取得
    //! @note		RenderTexutreとして使用される場合のみアクセス可能
    inline const RenderTextureDesc& VulkanTexture::descOfRenderTexture()const {
        return m_renderDesc;
    }

}
