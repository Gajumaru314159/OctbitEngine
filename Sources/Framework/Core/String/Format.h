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
#include <fmt/format.h>
#include <fmt/color.h>
#undef index
//! @endcond


namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  文字列をフォーマット
	//@―---------------------------------------------------------------------------
	template<typename TChar, class... Args>
	StringBase<TChar> Format(StringViewBase<TChar> fmt, Args&&... args) {
		return fmt::format(fmt, ob::forward<Args>(args)...);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  出力先を指定して文字列をフォーマット
	//@―---------------------------------------------------------------------------
	template<typename TOutItr, typename TChar, class... Args, typename = std::enable_if_t<!std::is_same_v<TOutItr, StringBase<TChar>>>>
	TOutItr FormatTo(TOutItr out, StringViewBase<TChar> fmt, Args&&... args) {
		return fmt::format_to(out, fmt, args...);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  出力先を指定して文字列をフォーマット
	//@―---------------------------------------------------------------------------
	template<typename TOutItr, typename TChar, class... Args>
	TOutItr FormatToN(TOutItr out, size_t length, StringViewBase<TChar> fmt, Args&&... args) {
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