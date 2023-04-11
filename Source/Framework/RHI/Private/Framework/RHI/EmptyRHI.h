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

		//@―---------------------------------------------------------------------------
		//! @brief      デバイス生成
		//@―---------------------------------------------------------------------------
		Device* getDevice() override { return nullptr; }

		//@―---------------------------------------------------------------------------
		//! @brief      
		//@―---------------------------------------------------------------------------
		GraphicObjectManager& getObjectManager() { return m_graphicObjectManager; }

	private:

		GraphicObjectManager m_graphicObjectManager;

	};

}// namespace pb::rhi