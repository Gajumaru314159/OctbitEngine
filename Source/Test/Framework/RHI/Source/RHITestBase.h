//***********************************************************
//! @file
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

struct DirectX12Tag {};
struct VulkanTag {};

template<typename T>
class RHITest : public ::testing::Test {
protected:
	virtual void SetUp() {

		ServiceInjector injector;
		ServiceContainer container;
		
		if constexpr (std::is_same_v<T, DirectX12Tag>) {
			rhi::RegisterDirectX12RHIService(injector);
		}
		if constexpr (std::is_same_v<T, VulkanTag>) {
			rhi::RegisterVulkanRHIService(injector);
		}

		rhi::RegisterRHIService(injector);

		m_dx12config.enableDebugLayer = true;
		m_dx12config.breakWithWarning = true;
		m_vkconfig.enableDebugLayer = true;
		injector.bind(m_config);
		injector.bind(m_dx12config);
		injector.bind(m_vkconfig);

		injector.create<Device>(m_container);

	};
	virtual void TearDown() {};

	//Logger m_logger;
	RHIConfig m_config;
	DirectX12RHIConfig m_dx12config;
	VulkanRHIConfig m_vkconfig;
	ServiceContainer m_container;
};


using RHIImplementations = ::testing::Types<DirectX12Tag, VulkanTag>;
TYPED_TEST_SUITE(RHITest, RHIImplementations);