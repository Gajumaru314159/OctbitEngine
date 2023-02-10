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
	template <typename S, typename TChar = fmt::char_t<S>, typename... Args>
	auto Format(const S& fmt, Args&&... args) -> std::basic_string<TChar> {
		return fmt::format(fmt, std::forward<Args>(args)...);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  出力先を指定して文字列をフォーマット
	//@―---------------------------------------------------------------------------
	template <typename OutputIt, typename S,typename TChar = fmt::char_t<S>,
		std::enable_if_t<fmt::detail::is_output_iterator<OutputIt, TChar>::value&&fmt::detail::is_exotic_char<TChar>::value>,
		typename... Args>
	inline auto FormatTo(OutputIt out, const S& fmt, Args&&... args) -> OutputIt {
		return fmt::format_to(out, fmt, args...);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  出力先を指定して文字列をフォーマット
	//@―---------------------------------------------------------------------------
	template <typename OutputIt, typename S,typename TChar = fmt::char_t<S>,
		std::enable_if_t<fmt::detail::is_output_iterator<OutputIt, TChar>::value&&fmt::detail::is_exotic_char<TChar>::value>,
		typename... Args>
	inline auto FormatToN(OutputIt out, size_t n, const S& fmt,const Args&... args) -> fmt::format_to_n_result<OutputIt> {
		return fmt::format_to_n(out, length, fmt, args...);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  文字列をフォーマットして保存するのに必要な文字数を返す
	//@―---------------------------------------------------------------------------
	template <typename S, typename TChar = fmt::char_t<S>,
		std::enable_if_t<fmt::detail::is_exotic_char<TChar>::value>, 
		typename... Args>
	inline auto FormattedSize(const S& fmt, const Args&... args) -> size_t {
		return fmt::formatted_size(fmt, args...);
	}

}// namespcae ob