//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>
#include <Framework/core/String/String.h>

//! @cond
// Check if fmt/format.h compiles with the X11 index macro defined.
#define index(x, y) no nice things

#pragma warning(push)
#pragma warning(disable:26451)
#pragma warning(disable:26495)
#pragma warning(disable:26498)
#pragma warning(disable:26812)
#include <fmt/xchar.h>
#include <fmt/color.h>
#pragma warning(pop)
#undef index
//! @endcond


namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  文字列をフォーマット
	//@―---------------------------------------------------------------------------
	template <typename S, typename TChar = fmt::char_t<S>, std::enable_if_t<!std::is_same<TChar, TChar>::value>, typename... Args>
	auto Format(const S& fmt, Args&&... args) -> StringBase<TChar> {
		return fmt::format(fmt, ob::forward<Args>(args)...);
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
	//! @brief		エラーコード表示用構造体
	//! 
	//! @details	u32 で表せるエラーコードを16進数表記でフォーマットする。
	//@―---------------------------------------------------------------------------
	struct ErrorCode {
		ErrorCode(u32 code)
			:code(code) {}
		u32 code;
	};

}// namespcae ob

template<> struct fmt::formatter<ob::core::ErrorCode> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}
	template <typename TFormatContext>
	auto format(ob::core::ErrorCode core, TFormatContext& ctx) -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), TC("0x{:08X}"),code.Value)
	}
};