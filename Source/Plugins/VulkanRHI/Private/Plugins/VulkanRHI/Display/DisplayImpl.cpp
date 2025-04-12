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
		vk::Win32SurfaceCreateInfoKHR info;
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

				
		vk::Extent2D size;
		size.width = std::clamp<u32>(desc.size.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		size.height = std::clamp<u32>(desc.size.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		
		if (desc.size.width != 0 && desc.size.height != 0) {
			size.width = desc.size.width;
			size.height = desc.size.height;
		} else if(desc.window) {
			size.width = desc.window.getSize().x;
			size.height = desc.window.getSize().y;
			// size = capabilities.currentExtent;
		} else {
			throw Exception("ウィンドウが指定されていません。");
		}

		// サイズをクランプ
		size.width = std::clamp(size.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		size.height = std::clamp(size.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);



		vk::PresentModeKHR swapchainPresentMode = vk::PresentModeKHR::eFifo;

		vk::SurfaceTransformFlagBitsKHR preTransform = 
			(capabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
			? vk::SurfaceTransformFlagBitsKHR::eIdentity
			: capabilities.currentTransform;

		vk::CompositeAlphaFlagBitsKHR compositeAlpha =
			(capabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
			: (capabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
			: (capabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit
			: vk::CompositeAlphaFlagBitsKHR::eOpaque;


		vk::SurfaceFormatKHR format = formats.at(0); // TODO desc.formatチェック

		// サーフェイス生成
		vk::SwapchainCreateInfoKHR swapChainCreateInfo;
		swapChainCreateInfo.flags = {};
		swapChainCreateInfo.surface = m_surface;
		swapChainCreateInfo.minImageCount = capabilities.minImageCount;
		swapChainCreateInfo.imageFormat = format.format;
		swapChainCreateInfo.imageColorSpace = format.colorSpace;
		swapChainCreateInfo.imageExtent.width = m_desc.size.width;
		swapChainCreateInfo.imageExtent.height = m_desc.size.height;
		swapChainCreateInfo.imageArrayLayers = 1;
		swapChainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
		swapChainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
		swapChainCreateInfo.queueFamilyIndexCount = 0;
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;
		swapChainCreateInfo.preTransform = preTransform;
		swapChainCreateInfo.compositeAlpha = compositeAlpha;
		swapChainCreateInfo.presentMode = swapchainPresentMode;
		swapChainCreateInfo.clipped = VK_TRUE;
		swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		m_swapchain = m_rhi.getDevice().createSwapchainKHR(swapChainCreateInfo, m_rhi.getAllocationCallbacks());


		// Image取得
		auto images = m_swapchain.getImages();

		// ImageView生成
		for (auto& image : images) {

			// m_textures.push_back(new TextureImpl(image));

			vk::ImageViewCreateInfo imageViewCreateInfo;
			imageViewCreateInfo.flags = {};
			imageViewCreateInfo.image = image;
			imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
			imageViewCreateInfo.format = format.format;
			imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			m_imageViews.push_back(m_rhi.getDevice().createImageView(imageViewCreateInfo, m_rhi.getAllocationCallbacks()));

			
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	DisplayImpl::~DisplayImpl() {
		LOG_INFO("削除");
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