﻿//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/System.h>
#include <Framework/RHI/EmptyRHI.h>
#include <Framework/RHI/GraphicObjectManager.h>
#include <Framework/RHI/SystemResourceModule.h>
#include <Framework/Core/Utility/DI.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief      システムをServiceInjectorに登録
	//@―---------------------------------------------------------------------------
	void Register(ServiceInjector& injector) {
		injector.bind<GraphicObjectManager>();
		injector.bind<SystemResourceModule>();
		injector.bind<EmptyRHI>().as<RHI>();
	}

}// namespace ob::rhi