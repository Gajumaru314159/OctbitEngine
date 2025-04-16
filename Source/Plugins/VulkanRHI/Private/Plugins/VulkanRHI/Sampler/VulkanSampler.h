//***********************************************************
//! @file
//! @brief		シェーダ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Sampler.h>

namespace ob::rhi::vulkan {

    //! @brief  サンプラー実装(DirectX12)
    class VulkanSampler :public Sampler {
    public:

        VulkanSampler(VulkanRHI& rhi,const SamplerDesc& desc);

        //! @brief      名前を取得
        const String& getName()const override { return m_name; }

    private:
        String m_name;
		vk::raii::Sampler m_sampler = nullptr;
    };

}