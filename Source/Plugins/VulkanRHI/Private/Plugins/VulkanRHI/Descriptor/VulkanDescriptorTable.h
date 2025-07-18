//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorTable.h>

namespace ob::rhi {

	class VulkanDescriptorLayout;

    //! @brief  デスクリプタ・テーブル実装(DirectX12)
    class VulkanDescriptorTable :public DescriptorTable {
    public:

        //! @brief              コンストラクタ
        //!
		//! @param device       デバイス
		//! @param type         デスクリプタに設定するリソースの種類
        //! @param elementNum   要素数
        VulkanDescriptorTable(VulkanRHI& rhi, const DescriptorTableDesc& desc);


		//! @brief      名前を取得
		const String& getName()const override;

		const DescriptorTableDesc& getDesc()const override { return m_desc; }


		//! @brief  リソースを設定
		//! @{
		bool setResource(s32 index, const Ref<Buffer>& resource) override;
		bool setResource(s32 index, const Ref<Texture>& resource) override;
		bool setResource(s32 index, const Ref<Sampler>& resource) override;
		//! @}

	public:

		void record(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipeline, s32 slot) const;

	private:

		bool tryGetRangeType(s32 index, const Ref<rhi::Buffer>& buffer, vk::DescriptorType& type) const;
		bool tryGetRangeType(s32 index, const Ref<rhi::Texture>& texture, vk::DescriptorType& type) const;
		bool tryGetRangeType(s32 index, const Ref<rhi::Sampler>& sampler, vk::DescriptorType& type) const;

	private:

		struct BufferElement {
			Ref<Buffer> resource;
		};
		struct TextureElement {
			Ref<Texture> resource;
			vk::raii::ImageView view;
		};
		struct SamplerElement {
			Ref<Sampler> resource;
		};

		using Element = Variant<BufferElement, TextureElement, SamplerElement>;

    private:
		VulkanRHI&				m_rhi;
		DescriptorTableDesc		m_desc;

		vk::raii::DescriptorPool m_pool = nullptr;
		vk::raii::DescriptorSet m_set = nullptr;
		Vector<Element>			m_elements;

		VulkanDescriptorLayout* m_layout = nullptr; // m_desc.layoutのキャッシュ
    };

}