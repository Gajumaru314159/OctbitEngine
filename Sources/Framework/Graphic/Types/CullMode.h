﻿//***********************************************************
//! @file
//! @brief		カリング方法
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief      カリング方法
	//@―---------------------------------------------------------------------------
	enum class CullMode :u32 {
		None,	//!< なし
		Front,	//!< 前面を非表示
		Back,	//!< 背面を非表示 
	};

}// namespcae ob::graphic