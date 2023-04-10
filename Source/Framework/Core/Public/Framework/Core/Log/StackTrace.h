﻿//***********************************************************
//! @file
//! @brief		スタックトレース
//! @author		Gajumaru
//***********************************************************
#pragma once
#include<vector>
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/String.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief      スタックトレース情報
	//@―---------------------------------------------------------------------------
	struct StackTraceElement {
		String name;
		String filename;
		s32 line;
	};

	//@―---------------------------------------------------------------------------
	//! @brief      スタックトレース
	//@―---------------------------------------------------------------------------
	class StackTrace {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		StackTrace();

		//@―---------------------------------------------------------------------------
		//! @brief      スタック情報を取得
		//@―---------------------------------------------------------------------------
		auto& elements()const { return m_stack; }

	private:
		std::vector<StackTraceElement> m_stack;
	};

}