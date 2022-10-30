//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/VulkanRHI/Device/Instance.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class DebugReportCallback
	{
	public:
		DebugReportCallback(::VkInstance instance,::VkDebugReportFlagsEXT flags,::PFN_vkDebugReportCallbackEXT callback)
			: m_instance(nullptr)
		{
			::VkDebugReportCallbackCreateInfoEXT info{};
			info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			info.flags = flags;
			info.pfnCallback = callback;

			if (Failed(::vkCreateDebugReportCallbackEXT(instance, &info, nullptr, &m_callback))) {
				return;
			}

			m_instance = instance;
		}

		~DebugReportCallback();

	private:

		::VkInstance m_instance;
		::VkDebugReportCallbackEXT	m_callback;

	};

}// namespcae ob