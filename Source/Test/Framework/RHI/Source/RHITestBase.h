//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Plugins/DirectX12RHI/DirectX12RHIConfig.h>
#include <Plugins/VulkanRHI/System.h>
#include <Plugins/VulkanRHI/VulkanRHIConfig.h>
#include <Framework/Platform/System.h>
#include <magic_enum.hpp>

using namespace ob;
using namespace ob::rhi;
using namespace ob::platform;

class RHITestBase : public ::testing::Test {
protected:
	virtual void SetUp() {

		ServiceInjector injector;
		ServiceContainer container;
		
		rhi::RegisterDirectX12RHIService(injector);
		//rhi::RegisterVulkanRHIService(injector);
		rhi::RegisterRHIService(injector);

		m_dx12config.enableDebugLayer = true;
		m_vkconfig.enableDebugLayer = true;
		injector.bind(m_config);
		injector.bind(m_dx12config);
		injector.bind(m_vkconfig);

		injector.create<RHI>(m_container);

	};
	virtual void TearDown() {};

	RHIConfig m_config;
	DirectX12RHIConfig m_dx12config;
	VulkanRHIConfig m_vkconfig;
	ServiceContainer m_container;
};