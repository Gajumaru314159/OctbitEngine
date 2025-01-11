//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/Path.h>
#include <filesystem>

namespace ob::core {

	namespace {
		//! @brief		std::filesystem::pathに変換
		static std::filesystem::path ToStdPath(StringView path) {
			return std::filesystem::u8path((std::string_view)path);
		}

		//! @brief		Stringに変換
		static String ToString(const std::filesystem::path& path) {
			return reinterpret_cast<const char*>(path.u8string().c_str());
		}
	}

	//! @brief		区切り文字を取得
	Char Path::Separator() {
		return '/';
	}

	//! @brief		パス文字列を正規化する
	//! @detailas	* lexically_normalを使用して正規化
	//!				* 区切り文字を/に統一
	String Path::Normalize(StringView path) {
		auto stdpath = ToStdPath(path);
		String str = ToString(stdpath.lexically_normal());
		str.replace('\\', '/');
		return str;
	}

	//! @brief		ファイルパスからファイル名を取得
	//! @details	StemとExtensionを結合したものを返します。
	//!				例：sample.txt
	String Path::FileName(StringView path) {
		return ToString(ToStdPath(path).filename());
	}

	//! @brief		ファイルパスからStemを取得
	//! @details	拡張子を除いたファイル名を返します。
	//!				例：sample
	String Path::Stem(StringView path) {
		return ToString(ToStdPath(path).stem());
	}

	//! @brief		ファイルパスから拡張子を取得
	//! @param		withDot	ドットを含めるか
	//! @details	例：.txt / txt
	String Path::Extension(StringView path, WithDot withDot) {
		auto pos = path.rfind('.');
		if (pos == path.npos) return {};
		if (!withDot) pos++;
		return String(path.substr(pos));
	}

	//! @brief		ファイルパスから親ディレクトリを取得
	String Path::Parent(StringView path, s32 level) {
		auto stdpath = ToStdPath(path);
		for (s32 i = 0; i < level; ++i) {
			stdpath = stdpath.parent_path();
		}
		return ToString(stdpath);
	}

	//! @brief		ファイルパスが絶対パスか
	//! @details	Windowsの場合、ドライブ名か//から始まる場合trueを返します。
	bool Path::IsAbsolute(StringView path) {
		return ToStdPath(path).is_absolute();
	}

	//! @brief		ファイルパスが相対パスか
	//! @details	絶対パスではない場合trueを返します。
	bool Path::IsRelative(StringView path) {
		return ToStdPath(path).is_relative();
	}

	//! @brief		ファイルパスが拡張子を持つか
	//! @details	extに拡張子を指定した場合は対象の拡張子を持つか判定します。
	bool Path::HasExtension(StringView path, StringView ext) {
		if (ext.empty()) {
			return !Extension(path,WithDot::Yes).empty();
		}
		else {
			return Extension(path,WithDot::Yes).ends_with(ext);
		}
	}

	//! @brief		ファイルパスの拡張子を変更
	//! @details	ファイルパスが拡張子を持たない場合はfalseを返します。
	String Path::ReplaceExtension(StringView path, StringView extension) {
		return ToString(ToStdPath(path).replace_extension(ToStdPath(extension)));
	}

	//! @brief		ファイルパスのステムを変更
	String Path::ReplaceStem(StringView path, StringView stem) {
		auto stdpath = ToStdPath(path);
		auto extension = stdpath.extension();
		return ToString(stdpath.replace_filename(ToStdPath(stem)).replace_extension(extension));
	}

	//! @brief		ファイルパスのファイル名を変更
	String Path::ReplaceFileName(StringView path, StringView fileName) {
		return ToString(ToStdPath(path).replace_filename(ToStdPath(fileName)));
	}

	//! @brief		ファイルパスの拡張子を削除
	String Path::RemoveExtension(StringView path) {
		auto pos = path.rfind('.');
		if (pos == path.npos) return String(path);
		return String(path.substr(0,pos));
	}

	//! @brief		ファイルパスのファイル名を削除
	String Path::RemoveFileName(StringView path) {
		auto pos = path.rfind(Separator());
		if (pos == path.npos) return String(path);
		return String(path.substr(0, pos+1));
	}

}