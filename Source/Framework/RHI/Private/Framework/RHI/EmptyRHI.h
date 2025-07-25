//***********************************************************
//! @file
//! @brief		空RHI
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/GraphicObjectManager.h>

namespace ob::rhi {

	//! @brief      空RHI
	//! @details	Headlessビルドなどで使用する空のRHI
	class EmptyRHI : public Device {
	public:

		EmptyRHI(GraphicObjectManager& manager,RHIConfig* config)
			: Device(manager,config)
		{

		}

	};

}