//***********************************************************
//! @file
//! @brief		デスクリプタ・テーブル実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/VulkanRHI/RootSignature/VulkanRootSignature.h>

namespace ob::rhi::vulkan {

    //! @brief  デスクリプタ・テーブル実装(DirectX12)
    class VulkanDescriptorTable :public DescriptorTable {
    public:

        //! @brief              コンストラクタ
        //!
		//! @param device       デバイス
		//! @param type         デスクリプタに設定するリソースの種類
        //! @param elementNum   要素数
        VulkanDescriptorTable(VulkanRHI& rhi, const Ref<RootSignature>& signature, s32 slot);
		
		VulkanDescriptorTable(VulkanRHI& rhi, const BindingSlot& desc);


		//! @brief      名前を取得
		const String& getName()const override;


		//! @brief  リソースを設定
		//! @{
		bool setResource(s32 index, const Ref<Buffer>& resource) override;
		bool setResource(s32 index, const Ref<Texture>& resource) override;
		bool setResource(s32 index, const Ref<Sampler>& resource) override;
		//! @}

		//! @brief  バインドレスハンドルに使用するインデックスを取得
		u32 getBindlessIndex(s32 index = 0)const override { return 0; }

	public:

		void record(vk::CommandBuffer commandBuffer, s32 slot) const;

	private:

		bool tryGetRangeType(s32 index, const Ref<rhi::Buffer>& buffer, vk::DescriptorType& type) const;
		bool tryGetRangeType(s32 index, const Ref<rhi::Texture>& texture, vk::DescriptorType& type) const;
		bool tryGetRangeType(s32 index, const Ref<rhi::Sampler>& sampler, vk::DescriptorType& type) const;

    private:
		VulkanRHI&			m_rhi;

		BindingSlot			m_desc;
		Ref<VulkanRootSignature>  m_signature;
		s32					m_slot = -1;

		String				m_name;


		using Element = Variant<Ref<Buffer>, Ref<Texture>, Ref<Sampler>>;

		Vector<Element> m_elemetns;

		vk::raii::DescriptorPool m_pool = nullptr;
		vk::raii::DescriptorSet m_set = nullptr;

    };

}