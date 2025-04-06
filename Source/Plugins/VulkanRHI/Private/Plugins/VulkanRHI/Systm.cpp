
//***********************************************************
//! @file
//! @brief		DirectXの内部型変換
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/Platform/System.h>
#include <Plugins/VulkanRHI/System.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>

namespace ob::rhi::vulkan
{
	//@―---------------------------------------------------------------------------
	//! @brief      システムをServiceInjectorに登録
	//@―---------------------------------------------------------------------------
	void RegisterVulkanRHIService(ServiceInjector& injector) {
		injector.bind<VulkanRHI>().as<RHI>();
		platform::RegisterPlatformService(injector);
	}


}