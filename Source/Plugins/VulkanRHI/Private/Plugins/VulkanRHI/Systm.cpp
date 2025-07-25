
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/Platform/System.h>
#include <Plugins/VulkanRHI/System.h>
#include <Plugins/VulkanRHI/VulkanDevice.h>

namespace ob::rhi
{
	//@―---------------------------------------------------------------------------
	//! @brief      システムをServiceInjectorに登録
	//@―---------------------------------------------------------------------------
	void RegisterVulkanRHIService(ServiceInjector& injector) {
		injector.bind<VulkanDevice>().as<Device>();
		platform::RegisterPlatformService(injector);
	}


}