//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Display/DisplayImpl.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Texture/TextureImpl.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <Framework/Platform/Window.h>

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	DisplayImpl::DisplayImpl(VulkanRHI& rhi, const DisplayDesc& desc)
		: m_rhi(rhi)
	{
		// 未指定の場合はwindowから取得
		if (desc.size.width <= 1 || desc.size.height <= 1) {
			m_desc.size = { (s32)desc.window.getSize().x,(s32)desc.window.getSize().y };
		}

		// デバイスごとのサーフェイス生成
#ifdef OS_WINDOWS
		vk::Win32SurfaceCreateInfoKHR info{};
		info.hinstance = GetModuleHandle(nullptr);
		info.hwnd = (HWND)desc.window.getHandle();

		m_surface = rhi.getInstance().createWin32SurfaceKHR(info, rhi.getAllocationCallbacks());
#else
		static_assert(true, "Surface is not implemented.");
#endif

		// サーフェイスのサポートをチェック
		if (!rhi.getPhysicalDevice().getSurfaceSupportKHR(0, m_surface)) {
			throw vk::InitializationFailedError("スワップチェーンがサポートされていません。");
		}
		

		
		// サーフェスの機能を取得
		auto capabilities = rhi.getPhysicalDevice().getSurfaceCapabilitiesKHR(m_surface);
		auto formats = rhi.getPhysicalDevice().getSurfaceFormatsKHR(m_surface);
		auto presentModeList = rhi.getPhysicalDevice().getSurfacePresentModesKHR(m_surface);

				
		// VkExtent2D swapchain_size{};
		// if (capabilities.currentExtent.width == 0xFFFFFFFF)
		// {
		// 	swapchain_size.width = context.swapchain_dimensions.width;
		// 	swapchain_size.height = context.swapchain_dimensions.height;
		// }
		// else
		// {
		// 	swapchain_size = capabilities.currentExtent;
		// }


		vk::SurfaceFormatKHR format = formats.at(0); // TODO desc.formatチェック

		// サーフェイス生成
		vk::SwapchainCreateInfoKHR swapchain_create_info;
		swapchain_create_info.flags = {};
		swapchain_create_info.surface = m_surface;

		swapchain_create_info.minImageCount = capabilities.minImageCount;
		swapchain_create_info.imageFormat = format.format;
		swapchain_create_info.imageColorSpace = format.colorSpace;
		swapchain_create_info.imageExtent.width = m_desc.size.width;
		swapchain_create_info.imageExtent.height = m_desc.size.height;
		swapchain_create_info.imageArrayLayers = 1;

		swapchain_create_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
		swapchain_create_info.imageSharingMode = vk::SharingMode::eExclusive;
		swapchain_create_info.queueFamilyIndexCount = 0;
		swapchain_create_info.pQueueFamilyIndices = nullptr;
		swapchain_create_info.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
		swapchain_create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

		swapchain_create_info.presentMode = vk::PresentModeKHR::eFifo;
		swapchain_create_info.clipped = VK_TRUE;
		swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

		m_swapchain = m_rhi.getDevice().createSwapchainKHR(swapchain_create_info, m_rhi.getAllocationCallbacks());


		// Image取得
		auto images = m_swapchain.getImages();

		// ImageView生成
		for (auto& image : images) {

			// m_textures.push_back(new TextureImpl(image));

			vk::ImageViewCreateInfo image_view_create_info;
			image_view_create_info.flags = {};
			image_view_create_info.image = image;

			image_view_create_info.viewType = vk::ImageViewType::e2D;
			image_view_create_info.format = vk::Format::eB8G8R8A8Unorm;
			image_view_create_info.components.r = vk::ComponentSwizzle::eIdentity;
			image_view_create_info.components.g = vk::ComponentSwizzle::eIdentity;
			image_view_create_info.components.b = vk::ComponentSwizzle::eIdentity;
			image_view_create_info.components.a = vk::ComponentSwizzle::eIdentity;

			image_view_create_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			image_view_create_info.subresourceRange.baseMipLevel = 0;
			image_view_create_info.subresourceRange.levelCount = 1;
			image_view_create_info.subresourceRange.baseArrayLayer = 0;
			image_view_create_info.subresourceRange.layerCount = 1;

			m_imageViews.push_back(m_rhi.getDevice().createImageView(image_view_create_info, m_rhi.getAllocationCallbacks()));

			
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	DisplayImpl::~DisplayImpl() {
	}


	//! @brief  妥当な状態か
	bool DisplayImpl::isValid()const {
		return !m_textures.empty();
	}


	//! @brief  定義を取得
	const DisplayDesc& DisplayImpl::getDesc()const noexcept {
		return m_desc;
	}

	//! @brief 更新
	void DisplayImpl::update() {

	}


	//! @brief      イベントリスナ追加
	void DisplayImpl::addEventListener(DisplayEventHandle& handle, DisplayEventDelegate func) {

	}

}