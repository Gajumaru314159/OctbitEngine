//***********************************************************
//! @file
//! @brief		RHI のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Plugins/VulkanRHI/System.h>
using namespace ob;
using namespace ob::rhi;

TEST(VulkanRHI, RHI) {

	Logger log;

	ServiceInjector injector;
	ServiceContainer container;
	ob::rhi::RegisterVulkanRHIService(injector);
	rhi::RegisterRHIService(injector);

	RHIConfig config;
	injector.bind(config);

	ServiceContainer m_container;
	injector.create<RHI>(container);
}