//***********************************************************
//! @file
//! @brief		コマンドプール
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/VulkanRHI/Command/CommandPool.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi::vulkan {


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	CommandPool::CommandPool(VkDevice device, u32 familyIndex)
		: m_device(device)
	{
		VkCommandPoolCreateInfo ci{};
		ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		ci.queueFamilyIndex = familyIndex;
		ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		if (Failed(::vkCreateCommandPool(m_device, &ci, nullptr, &m_commandPool))) {
			LOG_ERROR("VkCommandPoolの生成に失敗");
		}
	}

}// ob::rhi::vulkan 