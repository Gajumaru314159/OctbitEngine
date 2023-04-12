﻿//***********************************************************
//! @file
//! @brief		RHIシステム(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/Platform/System.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>

namespace ob::rhi::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief      システムをServiceInjectorに登録
	//@―---------------------------------------------------------------------------
	void Register(ServiceInjector& injector) {
		injector.bind<DirectX12RHI>().as<RHI>();
		platform::Register(injector);
	}

}