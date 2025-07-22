//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Sampler.h>
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorHandle.h>

namespace ob::rhi {

    //! @brief  サンプラー実装(Vulkan)
    class VulkanSampler :public Sampler {
    public:

        VulkanSampler(VulkanRHI& rhi,const SamplerDesc& desc);

        //! @brief      名前を取得
        const String& getName()const override { return m_name; }

        //! @brief      BindlessHandleを取得
        BindlessHandle getHandle()const override;

    public:

		//! @brief      サンプラーを取得
		vk::Sampler getNative() const { return m_sampler; }

    private:
        String m_name;
		vk::raii::Sampler m_sampler = nullptr;
        VulkanDescriptorHandle m_handle;
    };


	//!@ condn


	//! @brief      BindlessHandleを取得
	inline BindlessHandle VulkanSampler::getHandle()const {
        BindlessHandle handle;
        if (m_handle.empty()) return handle;
        handle.type = BindingType::Sampler;
        handle.index = m_handle.getBindlessIndex();
        return handle;
	}


	//! @endcond



}