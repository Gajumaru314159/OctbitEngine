//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Display.h>
#include <Plugins/VulkanRHI/Display/Surface.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::vulkan {

	class DisplayImpl :public Display{
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		DisplayImpl(VkInstance instance,VkPhysicalDevice physicalDevice, VkDevice device, const DisplayDesc& desc);
		~DisplayImpl();
		
		bool isValid()const override;		

		const String& getName()const override {
			return m_desc.name;
		}

		//! @brief      定義を取得
		const DisplayDesc& getDesc()const noexcept;

		//! @brief      更新
		void update();

		//! @brief      イベントリスナ追加
		void addEventListener(DisplayEventHandle& handle, DisplayEventDelegate func);

	private:

		DisplayDesc			m_desc;

		::VkInstance		m_instance;
		::VkPhysicalDevice	m_physicalDevice;
		::VkDevice			m_logicalDevice;

		::VkSurfaceKHR		m_surface;
		::VkSwapchainKHR    m_swapchain;
		Vector<VkImageView>	m_imageViews;
		//std::vector<vulkan::ImageView> m_image_views;
		//std::unique_ptr<vulkan::Queue> m_present_queue;
		//vulkan::Device* m_device;

	};
}






//===============================================================
// インライン
//===============================================================
namespace ob::rhi::dx12 {

}