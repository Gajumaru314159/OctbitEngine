//***********************************************************
//! @file
//! @brief		フォーマット関数
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <string>
#include <utility>
#include <format>
#include <Framework/Core/CoreTypes.h>

namespace ob::core {

	//! @brief  文字列をフォーマット
	template <typename TFormat, typename... TArgs>
	auto Format(TFormat&& format, TArgs&&... args) {
		return std::vformat(format, std::make_format_args(args...));
	}

	//! @brief  出力先を指定して文字列をフォーマット
	//! @return	OutputIterator
	template <typename TOut, typename TFormat, typename... TArgs>
	auto FormatTo(TOut&& out, TFormat&& format, TArgs&&... args) {
		return std::vformat_to(out, format, std::make_format_args(args...));
	}

	//! @brief  文字列をフォーマットして保存するのに必要な文字数を返す
	template <typename TFormat, typename... TArgs>
	auto FormattedSize(const TFormat& format, TArgs&&... args) {
		return std::formatted_size(format, std::make_format_args(args...));
	}

}