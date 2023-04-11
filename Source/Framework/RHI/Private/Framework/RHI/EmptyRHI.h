//***********************************************************
//! @file
//! @brief		空・RHI・モジュール
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/GraphicObjectManager.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief      空・RHI・モジュール
    //@―---------------------------------------------------------------------------
	class EmptyRHI : public RHI {
	public:

		EmptyRHI(GraphicObjectManager& manager)
			: RHI(manager)
		{

		}

	};

}// namespace pb::rhi