//***********************************************************
//! @file
//! @brief		シェーダ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/FrameBuffer.h>

namespace ob::rhi::vulkan {

    //! @brief  サンプラー実装(DirectX12)
    class VulkanFrameBuffer :public FrameBuffer {
    public:

        VulkanFrameBuffer(VulkanRHI& rhi,const FrameBufferDesc& desc);

        //! @brief      名前を取得
        const String& getName()const override { return m_name; }

    private:
        String m_name;
		vk::raii::Framebuffer m_frameBuffer = nullptr;
    };

}