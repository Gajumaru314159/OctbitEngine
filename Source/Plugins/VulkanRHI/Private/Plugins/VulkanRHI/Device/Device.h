//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Device/Device.h>

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class Device
	{
	public:
		Device(
			PhysicalDevice* phycical_device,
			Span<const char*> const& required_layer_names,
			Sapn<const char*> const& required_extension_names)
			
			: m_phycical_device(phycical_device)
		{
			std::vector<const char*> layer_names;
			auto const available_layer_names = EnumerateDeviceLayerNames(phycical_device);
			for (auto const& required_layer_name : required_layer_names)
			{
				if (std::ranges::find(
					available_layer_names,
					required_layer_name) != available_layer_names.end())
				{
					layer_names.push_back(required_layer_name);
				}
			}

			std::vector<const char*> extension_names;
			auto const available_extension_names = EnumerateDeviceExtensionNames(phycical_device);
			for (auto const& required_extension_name : required_extension_names)
			{
				if (std::ranges::find(
					available_extension_names,
					required_extension_name) != available_extension_names.end())
				{
					extension_names.push_back(required_extension_name);
				}
			}

			Create(layer_names, extension_names);
		}


	private:

		static Array<std::string> EnumerateDeviceLayerNames(vulkan::PhysicalDevice* phycical_device)
		{
			Array<std::string> layer_names;
			for (auto const& layer_property : phycical_device->EnumerateDeviceLayerProperties())
			{
				layer_names.push_back(layer_property.layerName);
			}
			return layer_names;
		}

		static Array<std::string> EnumerateDeviceExtensionNames(vulkan::PhysicalDevice* phycical_device)
		{
			Array<std::string> extension_names;
			for (auto const& extension_property : phycical_device->EnumerateDeviceExtensionProperties(nullptr))
			{
				extension_names.push_back(extension_property.extensionName);
			}
			for (auto const& layer_names : EnumerateDeviceLayerNames(phycical_device))
			{
				for (auto const& extension_property : phycical_device->EnumerateDeviceExtensionProperties(layer_names.c_str()))
				{
					extension_names.push_back(extension_property.extensionName);
				}
			}
			return extension_names;
		}

	private:


	private:

		::VkDevice              m_device;
		vulkan::PhysicalDevice* m_phycical_device;

	};

}// namespcae ob