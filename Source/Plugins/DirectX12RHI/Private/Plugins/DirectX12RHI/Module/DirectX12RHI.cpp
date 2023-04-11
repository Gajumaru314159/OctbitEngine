//***********************************************************
//! @file
//! @brief		RHI・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>

namespace ob::rhi::dx12 {

	DirectX12RHI::DirectX12RHI(ob::rhi::Config* config, platform::WindowManager&)
		: m_config(config ? *config : ob::rhi::Config{})
		, m_device(m_config)
		, m_objectManager(m_config.frameBufferCount)
	{

	}
	DirectX12RHI::~DirectX12RHI() {
	}
	Device* DirectX12RHI::getDevice() {
		return m_device.get();
	}

	void DirectX12RHI::update() {
		m_device->update();
		m_objectManager->update();
	}

}