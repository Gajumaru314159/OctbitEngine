//***********************************************************
//! @file
//! @brief		ステンシル・オペレータ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief      ステンシル・オペレータ
	//@―---------------------------------------------------------------------------
	enum class StencilOp :u32 {
		Keep,				//!< 			
		Zero,				//!< 
		Replace,			//!< 
		IncrementAndClamp,	//!< 
		DecrementAndClamp,	//!< 
		Invert,				//!< 
		IncrementAndWrap,	//!< 
		DecrementAndWrap,	//!< 
	};

}// namespcae ob::graphic