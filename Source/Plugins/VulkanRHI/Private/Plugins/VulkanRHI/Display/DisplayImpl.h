//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/IDisplay.h>
#include <Framework/RHI/Types/RenderPassDesc.h>
#include <Plugins/VulkanRHI/Display/Surface.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::vulkan {

	class DisplayImpl :public IDisplay{
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		DisplayImpl(VkInstance instance,VkPhysicalDevice physicalDevice, VkDevice device, const SwapchainDesc& desc);
		~DisplayImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


        //@―---------------------------------------------------------------------------
        //! @brief  定義を取得
        //@―---------------------------------------------------------------------------
        const SwapchainDesc& getDesc()const noexcept override;


        //@―---------------------------------------------------------------------------
        //! @brief  バックバッファのサイズを変更
        //@―---------------------------------------------------------------------------
        bool resizeBackBuffer(const Size& size) override;


        //@―---------------------------------------------------------------------------
        //! @brief      更新
        //@―---------------------------------------------------------------------------
        void update() override;

	private:

        SwapchainDesc m_desc;

		::VkInstance		m_instance;
		::VkPhysicalDevice	m_physicalDevice;
		::VkDevice			m_logicalDevice;

		::VkSurfaceKHR		m_surface;
		::VkSwapchainKHR    m_swapchain;
		Array<VkImageView>	m_imageViews;
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