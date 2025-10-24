//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorLayout.h>

namespace ob::rhi {

    //! @brief  デスクリプタ・テーブル実装(Vulkan)
    class VulkanDescriptorLayout :public DescriptorLayout {
    public:

        //! @brief              コンストラクタ
		VulkanDescriptorLayout(VulkanDevice& device,const DescriptorLayoutDesc& desc);

		const DescriptorLayoutDesc& getDesc() const override { return m_desc; }


		//! @brief      名前を取得
		const String& getName()const override { return m_desc.name; }

	public:

		vk::DescriptorSetLayout getNative() const {
			return *m_layout;
		}

    private:

		DescriptorLayoutDesc			m_desc;
		vk::raii::DescriptorSetLayout	m_layout = nullptr;

    };

}