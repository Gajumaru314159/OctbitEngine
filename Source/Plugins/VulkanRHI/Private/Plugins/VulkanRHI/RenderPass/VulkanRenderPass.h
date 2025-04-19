//***********************************************************
//! @file
//! @brief		シェーダ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RenderPass.h>

namespace ob::rhi::vulkan {

    //! @brief  サンプラー実装(DirectX12)
    class VulkanRenderPass :public RenderPass {
    public:

        VulkanRenderPass(VulkanRHI& rhi,const RenderPassDesc& desc);

        //! @brief      名前を取得
        const String& getName()const override { return m_name; }

    private:
        String m_name;
		vk::raii::RenderPass m_renderPass = nullptr;
    };

}