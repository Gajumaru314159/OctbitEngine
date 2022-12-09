//***********************************************************
//! @file
//! @brief		デバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/VulkanRHI/Utility/Utility.h>

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  デバイス
	//@―---------------------------------------------------------------------------

	class Surface
	{
	public:
		Surface(::VkInstance instance, ::HWND hwnd)
			: m_instance(instance)
		{
			OB_CHECK_ASSERT_EXPR(instance);

#ifdef OS_WINDOWS
			::VkWin32SurfaceCreateInfoKHR info{};
			info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			info.pNext = nullptr;
			info.hinstance = GetModuleHandle(nullptr);
			info.hwnd = hwnd;

			if (Failed(::vkCreateWin32SurfaceKHR(m_instance, &info, nullptr, &m_surface))) {
				OB_ASSERT("vkCreateWin32SurfaceKHRに失敗");
			}
#else
			static_assert(true, "Surface is not implemented.");
#endif
		}

		~Surface()
		{
			if (m_surface) {
				::vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
				m_surface = nullptr;
			}
		}

		auto getNative() { return m_surface; }

	private:
		::VkSurfaceKHR		m_surface;
		::VkInstance		m_instance;
	};

}// namespcae ob