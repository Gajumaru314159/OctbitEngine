//***********************************************************
//! @file
//! @brief		ラスタライズ・ステート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/FillMode.h>
#include <Framework/Graphic/Types/CullMode.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief      ラスタライズ・ステート
	//@―---------------------------------------------------------------------------
	struct RasterizerState{
		FillMode fillMode{FillMode::Solid};
		CullMode cullMode{ CullMode::Back };
	};

}// namespcae ob::graphic