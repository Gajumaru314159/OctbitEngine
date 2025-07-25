//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Framework/Graphics/All.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Plugins/DirectX12RHI/DirectX12RHIConfig.h>
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

struct DirectX12Tag{};
struct VulkanTag{};

template<typename T>
class GraphicsTestBase : public ::testing::Test {
protected:
	GraphicsTestBase(bool bindless) {
		m_config.enableBindless = bindless;
		m_dx12config.enableDebugLayer = true;
		m_dx12config.breakWithWarning = true;
		m_vkconfig.enableDebugLayer = true;
	}

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
		graphics::RegisterGraphicsService(injector);

		injector.bind(m_config);
		injector.bind(m_dx12config);
		injector.bind(m_vkconfig);

		injector.createAll<ob::rhi::SystemResource, ob::graphics::Graphics>(m_container);

	};
	virtual void TearDown() {};

public:
	RHIConfig m_config;
	DirectX12RHIConfig m_dx12config;
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


using RHIImplementations = ::testing::Types<DirectX12Tag, VulkanTag>;
TYPED_TEST_SUITE(BindlessGraphicsTest, RHIImplementations);
TYPED_TEST_SUITE(BindfullGraphicsTest, RHIImplementations);