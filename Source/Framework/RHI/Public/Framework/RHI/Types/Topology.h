﻿//***********************************************************
//! @file
//! @brief		トポロジー
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief      トポロジー
	//@―---------------------------------------------------------------------------
	enum class Topology :u32 {
		PointList,		//!< 頂点リスト
		LineList,		//!< 線分リスト
		TriangleList,	//!< 三角形リスト
	};

}// namespcae ob::rhi