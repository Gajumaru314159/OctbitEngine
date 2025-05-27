//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Sampler.h>

namespace ob::rhi {

    //! @brief  サンプラー実装(DirectX12)
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
    };


	//!@ condn


	//! @brief      BindlessHandleを取得
	inline BindlessHandle VulkanSampler::getHandle()const {
        return {};
	}


	//! @endcond



}