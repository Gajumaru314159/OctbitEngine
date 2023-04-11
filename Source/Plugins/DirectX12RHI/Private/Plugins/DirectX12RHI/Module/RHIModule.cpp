//***********************************************************
//! @file
//! @brief		RHI・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Module/RHIModule.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>

namespace ob::rhi::dx12 {

	DirectX12RHIModule::DirectX12RHIModule(platform::WindowManager&)
		: m_device()
		, m_objectManager(2)
	{

	}
	DirectX12RHIModule::~DirectX12RHIModule() {
	}
	Device* DirectX12RHIModule::getDevice() {
		return m_device.get();
	}

	void DirectX12RHIModule::update() {
		m_device->update();
		m_objectManager->update();
	}

}