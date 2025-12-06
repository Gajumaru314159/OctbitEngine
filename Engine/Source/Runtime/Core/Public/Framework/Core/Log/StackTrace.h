//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include<vector>
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/String.h>
#include <Framework/Core/Template/Container/Vector.h>

namespace ob::core {

	//! @brief      スタックトレース情報
	struct StackTraceElement {
		String name;
		String filename;
		s32 line;
	};

	//! @brief      スタック情報を取得
	struct StackTrace {
		static constexpr s32 MAX_DEPTH = 32;
		
		s32		depth = 0;
		void*	stack[MAX_DEPTH] = {};

		//! @brief      スタック情報をキャプチャ
		static StackTrace Capture(s32 frameToSkip = 0);

		//! @brief      スタック情報をStackTraceElementに変換して取得
		auto elements()const -> Vector<StackTraceElement>;

	};

	

}