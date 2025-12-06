//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include<vector>
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/String.h>
#include <Framework/Core/Template/Container/Vector.h>
#include <stacktrace>
#include "Framework/Core/Template/Container/FixedVector.h"

namespace ob::core {

	//! @brief      スタックトレース情報
	struct StackTraceElement {
		String name;
		String filename;
		s32 line;
	};

	//! @brief      スタック情報を取得
	struct StackTrace {

		static constexpr auto MAX_ENTRY_COUNT = 32;

		//! @brief      スタック情報のエントリ一覧
		FixedVector<std::stacktrace_entry,MAX_ENTRY_COUNT> entries;

		//! @brief      スタック情報をキャプチャ
		static StackTrace Capture(s32 frameToSkip = 0);

		//! @brief      スタック情報をStackTraceElementに変換して取得
		auto elements()const -> FixedVector<StackTraceElement,MAX_ENTRY_COUNT>;

	};

	

}
