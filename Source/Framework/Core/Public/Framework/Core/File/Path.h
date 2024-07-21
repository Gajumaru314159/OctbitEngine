//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <filesystem>
#include <Framework/Core/String/String.h>
#include <Framework/Core/String/Format.h>
#include <Framework/Core/Misc/YesNo.h>

namespace ob::core {
	
	DEFINE_YES_NO(WithDot);

	//@―---------------------------------------------------------------------------
	//! @brief		ファイルシステムのパスを表すクラス
	//! @details	このクラス自体はファイルシステムを操作しません。
	//!				操作する場合は File や Directory を使用してください。
	//!				このクラス内では暗黙的に以下の処理が行われます。
	//!				* ディレクトリ区切り文字を/に統一
	//@―---------------------------------------------------------------------------
	class Path {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		複数の文字列を結合してPath文字列に変換する。
		//@―---------------------------------------------------------------------------
		template<class... TArgs>
		static String Combine(TArgs&&... args) { return String((std::filesystem::path(std::string_view(args)) / ...).u8string()).replace('\\',Separator()); }

		//@―---------------------------------------------------------------------------
		//! @brief		区切り文字を取得
		//@―---------------------------------------------------------------------------
		static Char Separator();
		static String Normalize(StringView path);
		static String FileName(StringView path);
		static String Stem(StringView path);
		static String Extension(StringView path, WithDot withDot = WithDot::No);
		static String Parent(StringView path, s32 level);
		static bool IsAbsolute(StringView path);
		static bool IsRelative(StringView path);
		static bool HasExtension(StringView path, StringView ext = "");
		static String ReplaceExtension(StringView path, StringView extension);
		static String ReplaceStem(StringView path, StringView stem);
		static String ReplaceFileName(StringView path, StringView fileName);
		static String RemoveExtension(StringView path);
		static String RemoveFileName(StringView path);

	};

}