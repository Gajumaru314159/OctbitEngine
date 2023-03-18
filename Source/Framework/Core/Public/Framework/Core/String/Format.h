//***********************************************************
//! @file
//! @brief		フォーマット関数
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <string>
#include <utility>

#pragma warning(push,0)
#ifdef OS_WINDOWS
#include <codeanalysis/warnings.h>
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#endif
#include <fmt/xchar.h>
#pragma warning(pop)

#include <Framework/Core/CoreTypes.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  文字列をフォーマット
	//@―---------------------------------------------------------------------------
	template <typename TFormat,typename... TArgs>
	auto Format(TFormat&& format,TArgs&&... args) {
		return fmt::format(format,std::forward<TArgs>(args)...);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  出力先を指定して文字列をフォーマット
	//! @return	OutputIterator
	//@―---------------------------------------------------------------------------
	template <typename TOut,typename TFormat, typename... TArgs>
	auto FormatTo(TOut&& out,TFormat&& format, TArgs&&... args) {
		return fmt::format_to(out,format, std::forward<TArgs>(args)...);
	}


	//@―---------------------------------------------------------------------------
	//! @brief	出力先を指定して文字列をフォーマット
	//! @return	フォーマット結果
	//@―---------------------------------------------------------------------------
	template <typename TOut,typename TSize, typename TFormat, typename... TArgs>
	auto FormatToN(TOut&& out, TSize&& n,TFormat&& format, TArgs&&... args) {
		return fmt::format_to_n(out,n, format, std::forward<TArgs>(args)...);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  文字列をフォーマットして保存するのに必要な文字数を返す
	//@―---------------------------------------------------------------------------
	template <typename TFormat, typename... TArgs>
	auto FormattedSize(TFormat&& format, TArgs&&... args) {
		return fmt::formatted_size(format, std::forward<TArgs>(args)...);
	}

}// namespcae ob