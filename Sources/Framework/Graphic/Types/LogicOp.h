//***********************************************************
//! @file
//! @brief		論理演算
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      論理演算
    //! 
    //! @see        BlendState    
    //@―---------------------------------------------------------------------------
    enum class LogicOp :u32 {
		Clear,			//!< 0
		Set,			//!< 1
		Copy,			//!< src
		CopyInverted,	//!< ~src
		Noop,			//!< 何もしない
		Invert,			//!< ~dst
		And,			//!< dst & src
		Nand,			//!< ~(dst & src)
		Or,				//!< dst | src
		Nor,			//!< ~(dst | src)
		Xor,			//!< dst ^ src
		Equivalent,		//!< ~(dst ^ src)
		AndReverse,		//!< src & ~dst
		AndInverted,	//!< ~src & dst
		OrReverse,		//!< dst | ~dst
		OrInverted,		//!< ~src | dst
    };

}// namespcae ob::graphic