//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/Platform/System.h>
#include <Plugins/DirectX12RHI/DirectX12Device.h>

namespace ob::rhi {

	//! @brief      システムをServiceInjectorに登録
	void RegisterDirectX12RHIService(ServiceInjector& injector) {
		DirectX12Device::Inject(injector);
	}

}