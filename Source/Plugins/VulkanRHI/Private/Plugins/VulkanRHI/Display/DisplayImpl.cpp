//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Display/DisplayImpl.h>
#include <Plugins/VulkanRHI/Device/DeviceImpl.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <Framework/Platform/Window.h>

namespace ob::rhi::vulkan {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    DisplayImpl::DisplayImpl(VkInstance instance,VkPhysicalDevice physicalDevice, VkDevice device, const SwapchainDesc& desc)
        : m_desc(desc)
		, m_instance(instance)
		, m_physicalDevice(physicalDevice)
		, m_logicalDevice(device)
    {
		if (desc.size.width == 0 || desc.size.height == 0) {
			m_desc.size = desc.window.getSize();
		}

		// デバイスごとのサーフェイス生成
#ifdef OS_WINDOWS
		::VkWin32SurfaceCreateInfoKHR info{};
		info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		info.pNext = nullptr;
		info.hinstance = GetModuleHandle(nullptr);
		info.hwnd = (HWND)desc.window.getHandle();

		ThrowIfFailed(::vkCreateWin32SurfaceKHR(m_instance, &info, nullptr, &m_surface));
#else
		static_assert(true, "Surface is not implemented.");
#endif

		// サーフェイスのサポートをチェック
		VkBool32 surfaceSupport = false;
		::vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, 0, m_surface, &surfaceSupport);
		if (surfaceSupport==false) {
			LOG_FATAL("スワップチェーンがサポートされていません。");
		}

		// サーフェスの機能を取得
		VkSurfaceCapabilitiesKHR capabilities;
		::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &capabilities);

		// 利用可能なフォーマットを取得
		uint32_t formatCount = 0;
		::vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr);
		Array<VkSurfaceFormatKHR> formats(formatCount);
		::vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, formats.data());

		//利用可能なプレゼンテーションモード
		uint32_t presentModeCount;
		::vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);
		Array<VkPresentModeKHR> presentModeList(presentModeCount);
		::vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModeList.data());



		VkSurfaceFormatKHR format = formats[0];

		// サーフェイス生成
		VkSwapchainCreateInfoKHR swapchain_create_info = {};
		swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchain_create_info.pNext = nullptr;
		swapchain_create_info.flags = 0;
		swapchain_create_info.surface = m_surface;

		swapchain_create_info.minImageCount = capabilities.minImageCount;
		swapchain_create_info.imageFormat = format.format;// TypeConverter::Convert(desc.format);
		swapchain_create_info.imageColorSpace = format.colorSpace;
		swapchain_create_info.imageExtent.width = m_desc.size.width;
		swapchain_create_info.imageExtent.height = m_desc.size.height;
		swapchain_create_info.imageArrayLayers = 1;

		swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchain_create_info.queueFamilyIndexCount = 0;
		swapchain_create_info.pQueueFamilyIndices = nullptr;
		swapchain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		swapchain_create_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
		swapchain_create_info.clipped = VK_TRUE;
		swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;


		ThrowIfFailed(::vkCreateSwapchainKHR(m_logicalDevice, &swapchain_create_info, nullptr, &m_swapchain));



		// Image取得
		uint32_t imageCount;
		vkGetSwapchainImagesKHR(m_logicalDevice, m_swapchain, &imageCount, nullptr);
		Array<VkImage> images(imageCount);
		vkGetSwapchainImagesKHR(m_logicalDevice, m_swapchain, &imageCount, images.data());

		// ImageView生成
		for (auto& image: images) {

			VkImageViewCreateInfo image_view_create_info{};
			image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			image_view_create_info.pNext = nullptr;
			image_view_create_info.flags = 0;
			image_view_create_info.image = image;

			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = VK_FORMAT_B8G8R8A8_UNORM;
			image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.baseMipLevel = 0;
			image_view_create_info.subresourceRange.levelCount = 1;
			image_view_create_info.subresourceRange.baseArrayLayer = 0;
			image_view_create_info.subresourceRange.layerCount = 1;

			auto& imageView = m_imageViews.emplace_back();
			::vkCreateImageView(m_logicalDevice, &image_view_create_info, nullptr, &imageView);
		}
    }

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
    DisplayImpl::~DisplayImpl() {

		for (auto imageView : m_imageViews) {
			vkDestroyImageView(m_logicalDevice, imageView, nullptr);
		}
		m_imageViews.clear();

		if (m_swapchain) {
			::vkDestroySwapchainKHR(m_logicalDevice, m_swapchain, nullptr);
			m_swapchain = nullptr;
		}

		if (m_surface) {
			::vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
			m_surface = nullptr;
		}

		m_instance = nullptr;
		m_logicalDevice = nullptr;

    }


    //@―---------------------------------------------------------------------------
    //! @brief  妥当な状態か
    //@―---------------------------------------------------------------------------
    bool DisplayImpl::isValid()const {
        return m_swapchain;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  定義を取得
    //@―---------------------------------------------------------------------------
    const SwapchainDesc& DisplayImpl::getDesc()const noexcept {
        return m_desc;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファのサイズを変更
    //@―---------------------------------------------------------------------------
    bool DisplayImpl::resizeBackBuffer(const Size& size) {
        return false;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //@―---------------------------------------------------------------------------
    void DisplayImpl::update() {

    }

}