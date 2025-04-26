//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Plugins/VulkanRHI/System.h>
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
		
		rhi::dx12::RegisterDirectX12RHIService(injector);
		//rhi::vulkan::RegisterVulkanRHIService(injector);
		rhi::RegisterRHIService(injector);

		m_config.enableDebugLayer = true;
		injector.bind(m_config);

		injector.create<RHI>(m_container);

	};
	virtual void TearDown() {};

	RHIConfig m_config;
	ServiceContainer m_container;
};