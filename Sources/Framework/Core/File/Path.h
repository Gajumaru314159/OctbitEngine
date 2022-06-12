//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <filesystem>
#include <Framework/Core/String/StringEncoder.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  パス
    //@―---------------------------------------------------------------------------
    using Path = std::filesystem::path;

}// namespcae ob


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::Path, ob::core::Char> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const ob::core::Path& path, FormatContext& ctx) -> decltype(ctx.out()) {
		ob::core::String path2;
		ob::core::StringEncoder::Encode(path.native(), path2);
		return format_to(ctx.out(), TC("{}"), path2);
	}
};
//! @endcond