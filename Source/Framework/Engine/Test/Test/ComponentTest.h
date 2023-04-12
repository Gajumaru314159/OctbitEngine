//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>

namespace ob::test {

	//@―---------------------------------------------------------------------------
	//! @brief  コンポーネント
	//@―---------------------------------------------------------------------------
	class ComponentTest: public ob::engine::Component {
		friend class Entity;
	public:

		OB_RTTI();

		ComponentTest() {

		}

	};

}
