//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Display.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/Core/Utility/Swapper.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::vulkan {

	class VulkanRHI;

	class DisplayImpl :public Display{
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		DisplayImpl(VulkanRHI& rhi, const DisplayDesc& desc);
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

		VulkanRHI&					m_rhi;
		DisplayDesc					m_desc;

		vk::raii::SurfaceKHR		m_surface = nullptr;
		vk::raii::SwapchainKHR		m_swapchain = nullptr;
		Vector<vk::raii::ImageView>	m_imageViews;

		Swapper<Ref<RenderTexture>> m_textures;

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