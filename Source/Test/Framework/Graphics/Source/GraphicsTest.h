//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Framework/Graphics/All.h>
#ifdef OS_WINDOWS
#include <Plugins/DirectX12RHI/System.h>
#include <Plugins/DirectX12RHI/DirectX12RHIConfig.h>
#endif
#include <Plugins/VulkanRHI/System.h>
#include <Plugins/VulkanRHI/VulkanRHIConfig.h>
#include <Framework/Platform/System.h>
#include <magic_enum.hpp>

using namespace ob;
using namespace ob::rhi;
using namespace ob::platform;

namespace ob::rhi {
	class SystemResource;
}

#ifdef OS_WINDOWS
struct DirectX12Tag{};
#endif
struct VulkanTag{};

template<typename T>
class GraphicsTestBase : public ::testing::Test {
protected:
	GraphicsTestBase(bool bindless) {
		m_config.enableBindless = bindless;
    #ifdef OS_WINDOWS
		m_dx12config.enableDebugLayer = true;
		m_dx12config.breakWithWarning = true;
    #endif
		m_vkconfig.enableDebugLayer = true;
	}

	virtual void SetUp() {

		ServiceInjector injector;
		ServiceContainer container;

#ifdef OS_WINDOWS
		if constexpr (std::is_same_v<T, DirectX12Tag>) {
			rhi::RegisterDirectX12RHIService(injector);
		}
#endif
		if constexpr (std::is_same_v<T, VulkanTag>) {
			rhi::RegisterVulkanRHIService(injector);
		}

		rhi::RegisterRHIService(injector);
		graphics::RegisterGraphicsService(injector);

		injector.bind(m_config);
#ifdef OS_WINDOWS
		injector.bind(m_dx12config);
#endif
		injector.bind(m_vkconfig);

		injector.createAll<ob::rhi::SystemResource, ob::graphics::Graphics>(m_container);

	};
	virtual void TearDown() {};

public:
	RHIConfig m_config;
#ifdef OS_WINDOWS
	DirectX12RHIConfig m_dx12config;
#endif
	VulkanRHIConfig m_vkconfig;
	ServiceContainer m_container;
};

template<typename T>
class BindlessGraphicsTest : public GraphicsTestBase<T> {
public:
	BindlessGraphicsTest() : GraphicsTestBase<T>(true) {
	}
};
template<typename T>
class BindfullGraphicsTest : public GraphicsTestBase<T> {
public:
	BindfullGraphicsTest() : GraphicsTestBase<T>(false) {
	}
};


#ifdef OS_WINDOWS
using RHIImplementations = ::testing::Types<DirectX12Tag, VulkanTag>;
#else
using RHIImplementations = ::testing::Types<VulkanTag>;
#endif
TYPED_TEST_SUITE(BindlessGraphicsTest, RHIImplementations);
TYPED_TEST_SUITE(BindfullGraphicsTest, RHIImplementations);
