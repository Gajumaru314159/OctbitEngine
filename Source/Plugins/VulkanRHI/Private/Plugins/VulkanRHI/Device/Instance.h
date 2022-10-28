//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/VulkanRHI/Utility/Utility.h>

namespace ob::graphic::vulkan {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class Instance {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        Instance(StringView appName, Span<const char*> layerNames,Span<const char*> extensionNames) 
			: m_instance(nullptr)
		{
			// 使用可能な名前でフィルタ
			const auto existExtensionNames = EnumerateInstanceExtensionNames();
			const auto existLayerNames = EnumerateInstanceExtensionNames();

			Array<const char*> validExtensionNames;
			Array<const char*> validLayerNames;

			for (const auto& name : existExtensionNames)
			{
				auto found = std::find(extensionNames.begin(), extensionNames.end(), name);
				if (found != extensionNames.end()) {
					validExtensionNames.push_back(name.c_str());
				}
			}

			for (const auto& name : validLayerNames)
			{
				auto found = std::find(layerNames.begin(), layerNames.end(), name);
				if (found != layerNames.end()) {
					validLayerNames.push_back(name);
				}
			}

			// インスタンス生成
			std::string appName8;
			StringEncoder::Encode(appName, appName8);

			::VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = appName8.c_str();
			appInfo.pEngineName = appName8.c_str();
			appInfo.apiVersion = VK_API_VERSION_1_0;
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

			::VkInstanceCreateInfo instInfo{};
			instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instInfo.pApplicationInfo = &appInfo;
			instInfo.enabledExtensionCount = (uint32_t)validExtensionNames.size();
			instInfo.ppEnabledExtensionNames = validExtensionNames.data();
			instInfo.enabledLayerCount = (uint32_t)validLayerNames.size();
			instInfo.ppEnabledLayerNames = validLayerNames.data();

			if (Failed(::vkCreateInstance(&instInfo, nullptr, &m_instance)))return;

        }

		//@―---------------------------------------------------------------------------
		//! @brief  デストラクタ
		//@―---------------------------------------------------------------------------
		~Instance();

		::VkInstance Get() {
			return m_instance;
		}

		Array<::VkPhysicalDevice> enumeratePhysicalDevices()
		{
			std::uint32_t gpuCount;
			if (Failed(::vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr))) {
				OB_ASSERT("vkEnumeratePhysicalDevicesに失敗");
			}
			Array<::VkPhysicalDevice> gpus(gpuCount);
			if (Failed(::vkEnumeratePhysicalDevices(m_instance, &gpuCount, gpus.data()))) {
				OB_ASSERT("vkEnumeratePhysicalDevicesに失敗");
			}
			return gpus;
		}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
		::VkSurfaceKHR CreateSurface(::VkWin32SurfaceCreateInfoKHR const& info)
		{
			::VkSurfaceKHR surface;
			if (Failed(::vkCreateWin32SurfaceKHR(m_instance, &info, nullptr, &surface))) {
				OB_ASSERT("vkCreateWin32SurfaceKHRに失敗");
			}
			return surface;
		}
#endif

		void DestroySurface(::VkSurfaceKHR const& surface)
		{
			::vkDestroySurfaceKHR(m_instance, surface, nullptr);
		}

    private:

		//@―---------------------------------------------------------------------------
		//! @brief  
		//@―---------------------------------------------------------------------------
        static Array<std::string> EnumerateInstanceLayerNames()
        {
            Array<std::string> layerNames;
            for (auto const& properties : EnumerateInstanceLayerProperties())
            {
                layerNames.push_back(properties.layerName);
            }
            return layerNames;
        }

		//@―---------------------------------------------------------------------------
		//! @brief  
		//@―---------------------------------------------------------------------------
		static Array<std::string> EnumerateInstanceExtensionNames()
		{
			Array<std::string> extensionNames;
			for (auto const& properties : EnumerateInstanceExtensionProperties(nullptr))
			{
				extensionNames.push_back(properties.extensionName);
			}
			for (auto const& name : EnumerateInstanceLayerNames())
			{
				for (auto const& properties : EnumerateInstanceExtensionProperties(name.c_str()))
				{
					extensionNames.push_back(properties.extensionName);
				}
			}
			return extensionNames;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  
		//@―---------------------------------------------------------------------------
		static Array<::VkLayerProperties> EnumerateInstanceLayerProperties()
		{
			std::uint32_t propertyCount;
			if (Failed(::vkEnumerateInstanceLayerProperties(&propertyCount, nullptr))) {
				return {};
			}

			Array<::VkLayerProperties> properties(propertyCount);
			if (Failed(::vkEnumerateInstanceLayerProperties(&propertyCount, properties.data()))) {
				return {};
			}

			return properties;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  
		//@―---------------------------------------------------------------------------
		static Array<::VkExtensionProperties> EnumerateInstanceExtensionProperties(const char* layerName)
		{
			std::uint32_t propertyCount;
			if (Failed(::vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, nullptr))) {
				return {};
			}
			Array<::VkExtensionProperties> properties(propertyCount);
			if(Failed(::vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, properties.data()))){
				return {};
			}
			return properties;
		}

    private:

        ::VkInstance m_instance;

    };

}// namespcae ob