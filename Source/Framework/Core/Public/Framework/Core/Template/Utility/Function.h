﻿//***********************************************************
//! @file
//! @brief		関数
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <functional>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief      関数オブジェクト
	//@―---------------------------------------------------------------------------
	template<class SIGNATURE>
	using Func = std::function<SIGNATURE>;

	//@―---------------------------------------------------------------------------
	//! @brief      評価関数オブジェクト
	//@―---------------------------------------------------------------------------
	template<class... TArgs>
	using Pred = std::function<bool(TArgs...)>;

	//@―---------------------------------------------------------------------------
	//! @brief      参照オブジェクト
	//@―---------------------------------------------------------------------------
	template<class T>
	using ReferenceWrapper = std::reference_wrapper<T>;

}