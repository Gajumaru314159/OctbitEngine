//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

	class VulkanDevice;

    inline bool Failed(::VkResult res)
    {
        if (res < 0)
        {
            LOG_ERROR("Vulkanエラー{}",static_cast<s32>(res));
            return true;
        }
        return false;
    }

    inline bool Success(::VkResult res)
    {
        if (res < 0)
        {
            LOG_ERROR("Vulkanエラー{}", static_cast<s32>(res));
            return false;
        }
        return true;
    }


    inline ::VkResult ThrowIfFailed(::VkResult res)
    {
        if (res < 0)
        {
            throw std::runtime_error("Error Vulkan");
        }
        return res;
    }

}