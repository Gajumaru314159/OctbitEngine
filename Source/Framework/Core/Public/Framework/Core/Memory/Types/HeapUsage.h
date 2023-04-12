//***********************************************************
//! @file
//! @brief		ヒープ・使用法
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>

namespace ob::core {
	//@―---------------------------------------------------------------------------
	//! @brief ヒープ・使用法
	//@―---------------------------------------------------------------------------
	enum class HeapUsage :u32 {
		VirtualMemory = 0,      //!< 仮想メモリ
		CPUCached,              //!< CPU側キャッシュ

		GPUReadable,            //!< GPU読み込み
		GPUReadWritable,        //!< GPU読み書き

		Max,					//!< 最大値
	};

}