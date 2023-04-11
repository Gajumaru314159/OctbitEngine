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

		EmptyRHI()
			: m_graphicObjectManager(2)
		{

		}

	private:

		GraphicObjectManager m_graphicObjectManager;

	};

}// namespace pb::rhi