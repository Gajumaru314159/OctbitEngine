//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Plugins/DirectX12RHI/System.h>

using namespace ob;
using namespace ob::rhi;

class RHITestBase : public ::testing::Test {
protected:
	virtual void SetUp() {

		ServiceInjector injector;
		ServiceContainer container;
		rhi::dx12::RegisterDirectX12RHIService(injector);
		rhi::RegisterRHIService(injector);

		m_config.enableDebugLayer = true;
		injector.bind(m_config);

		injector.create<RHI>(m_container);

	};
	virtual void TearDown() {};

	// Logger m_logger;
	RHIConfig m_config;
	ServiceContainer m_container;
};