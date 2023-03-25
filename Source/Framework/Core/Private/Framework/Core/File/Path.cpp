//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/Path.h>
#include <Framework/Core/Platform/WindowsHeaders.h>
#include <Framework/Core/String/StringEncoder.h>

namespace ob::core {

	static StringView RemovePreSeparator(StringView path) {
		if (path.starts_with(Path::Separator())) {
			return path.substr(1);
		} else {
			return path;
		}
	}

	static StringView RemovePostSeparator(StringView path) {
		if (path.ends_with(Path::Separator())) {
			return path.substr(0, path.size() - 1);
		} else {
			return path;
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief		カレントディレクトリの絶対パスを取得
	//@―---------------------------------------------------------------------------
	Path Path::Curren() {
		StringBase<std::filesystem::path::value_type> nativeString = std::filesystem::current_path().native();
		String result;
		StringEncoder::Encode(nativeString, result);
		return Path(result);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		区切り文字を取得
	//@―---------------------------------------------------------------------------
	Char Path::Separator() {
		return TC('/');
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コピー代入
	//@―---------------------------------------------------------------------------
	Path& Path::operator = (const Path& rhs) {
		m_string = rhs.m_string;
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		パス要素を追加
	//@―---------------------------------------------------------------------------
	Path& Path::operator /= (const Path& rhs) {
		if (!rhs.empty()) {
			if (rhs.isRoot()) {
				m_string = rhs.m_string;
			} else {

				if (m_string.ends_with(Separator())) {
					m_string += RemovePreSeparator(rhs.m_string);
				} else {
					m_string += Separator();
					m_string += RemovePreSeparator(rhs.m_string);
				}

			}
		}
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		パス要素を追加
	//@―---------------------------------------------------------------------------
	Path Path::operator / (const Path& rhs)const {
		return Path{ *this } /= rhs;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		クリア
	//@―---------------------------------------------------------------------------
	void Path::clear() {
		m_string.clear();
	}

	//@―---------------------------------------------------------------------------
	//! @brief		拡張子を変更
	//! @details	Path("sample.txt").replaceExtension("md");	// sample.md
	//!				Path("sample.txt").replaceExtension(".md"); // sample.md
	//@―---------------------------------------------------------------------------
	Path& Path::replaceExtension(StringView extension) {
		removeExtension();
		if (!extension.empty()) {
			if (extension.starts_with(TC("."))) {
				m_string += extension;
			} else {
				m_string = m_string + TC(".") + extension;
			}
		}
		validate();
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		ステムを変更
	//! @details	Path("sample.txt").replaceStem("replaced"); // replaced.txt
	//@―---------------------------------------------------------------------------
	Path& Path::replaceStem(StringView stem) {
		String ext(extension());
		removeFileName();
		(*this) /= stem;
		replaceExtension(ext);
		validate();
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		ファイル名を変更
	//! @details	Path("sample.txt").replaceBaseName("replaced.md"); // replaced.md
	//@―---------------------------------------------------------------------------
	Path& Path::replaceFileName(StringView fileName) {
		removeFileName();
		(*this) /= fileName;
		validate();
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		拡張子を削除
	//! @details	Path("sample.txt").removeExtension();	// sample
	//@―---------------------------------------------------------------------------
	Path& Path::removeExtension() {
		m_string.pop_back_n(extension(WithDot::Yes).size());
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		ファイル名を削除
	//! @details	Path("dir/sample.txt").removeFileName();// dir/
	//!				Path("dir/sample").removeFileName();	// dir/
	//!				Path("dir/sub/").removeFileName();		// dir/sub
	//@―---------------------------------------------------------------------------
	Path& Path::removeFileName() {
		m_string.pop_back_n(fileName().size());
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		内部文字列にアクセス
	//! @details	内部文字列はパスとして使用できる状態を保証するために読み取り専用です。
	//@―---------------------------------------------------------------------------
	const String& Path::string()const {
		return m_string;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		空パスか
	//@―---------------------------------------------------------------------------
	bool Path::empty()const {
		return m_string.empty();
	}

	//@―---------------------------------------------------------------------------
	//! @brief		パスが絶対パスか
	//! @details	Windowsの場合、ドライブ名か//から始まる場合trueを返します。
	//!				UNIX系の場合、/から始まる場合trueを返します。
	//@―---------------------------------------------------------------------------
	bool Path::isAbsolute()const {

#ifdef OS_WINDOWS
		// UNCパス
		if (m_string.starts_with(StringView(TC("//")))) {
			return true;
		}
		// DOSパス C:
		if (2 <= m_string.size()) {
			auto volume = m_string[0];
			if ('A' <= volume && volume <= 'Z') {
				if (m_string[1] == ':') {
					return true;
				}
			}
		}
#else
		if (m_string.starts_with(TC('/')))) {
			return true;
		}
#endif
		return false;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		パスが相対パスか
	//! @details	絶対パスではない場合trueを返します。
	//!				Windowsの場合../や./から始まる場合、/から始まりUNCパスではない場合trueを返します。
	//!				UNIX系の場合/から始まらない場合trueを返します。
	//@―---------------------------------------------------------------------------
	bool Path::isRelative()const {
		return !isAbsolute();
	}

	//@―---------------------------------------------------------------------------
	//! @brief		ルートディレクトリか
	//! @details	Windowsの場合ドライブ名か//から始まるコンピュータ名の場合trueを返します。
	//!				UNIX系の場合/の場合trueを返します。
	//@―---------------------------------------------------------------------------
	bool Path::isRoot()const {

		if (!isAbsolute()) {
			return false;
		}

#ifdef OS_WINDOWS
		// UNCパス
		if (m_string.starts_with(StringView(TC("//")))) {
			if (m_string.find(TC('/'), 2) == m_string.npos) {
				return true;
			} else {
				return false;
			}
		}
		// DOSパス
		if (2 <= m_string.size()) {
			auto volume = m_string[0];
			if ('A' <= volume && volume <= 'Z' && m_string[1] == TC(':')) {
				// C:
				if (m_string.size() == 2) {
					return true;
				}
				// C:/
				if (m_string.size()==3 && m_string[2] == TC('/')) {
					return true;
				}
			}
		}
#else
		if (m_string == TC("/")) {
			return true;
		}
#endif
		return false;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		Directoryか	
	//! @details	パスが区切り文字で終わっているかルートの場合trueを返します。
	//@―---------------------------------------------------------------------------
	bool Path::isDirectory()const {
		if (m_string.ends_with(TC('/')))return true;
		if (isRoot())return true;
		return false;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		指定した拡張子を持つか
	//! @details	extに拡張子を指定した場合は対象の拡張子を持つか判定します。
	//!				.は省略可能です。
	//@―---------------------------------------------------------------------------
	bool Path::hasExtension(StringView ext)const {
		if (ext.empty()) {
			return !extension(WithDot::Yes).empty();
		} else {
			return extension(WithDot::Yes).ends_with(ext);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief		ファイル名を取得します。
	//! @details	Path("dir/sample.txt").fileName();	// sample.txt
	//!				Path("dir/sample").fileName();		// sample
	//!				Path("dir/sub/").fileName();		// 
	//@―---------------------------------------------------------------------------
	StringView Path::fileName()const {

		if (isDirectory()) {
			return {};
		}

		auto pos = m_string.rfind(TC('/'));
		if (pos == m_string.npos) {
			return m_string;
		}

		return m_string.substr(pos + 1);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		拡張子を除いたファイル名を取得します。
	//! @details	Path("C:/dir/sample.txt").stem();	// sample
	//@―---------------------------------------------------------------------------
	StringView Path::stem()const {

		auto name = fileName();
		auto pos = name.rfind(TC('.'));
		if (pos == m_string.npos) {
			return name;
		} else {
			return name.substr(0, pos);
		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief		拡張子を取得します。
	//! @details	Path("dir/sample.txt").extension();				// txt
	//!				Path("dir/sample.txt").extension(WithDot::Yes);	// .txt
	//!				Path("dir/sample").extension();					//
	//@―---------------------------------------------------------------------------
	StringView Path::extension(WithDot withDot)const {

		auto name = fileName();
		auto pos = name.rfind(TC('.'));
		if (pos == m_string.npos) {
			return {};
		}
		if (withDot) {
			return name.substr(pos);
		} else {
			return name.substr(pos + 1);
		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief		親ディレクトリまでのパスを取得します。
	//! @details	Path("C:/dir/sample.txt").baseName();	// C:/dir
	//@―---------------------------------------------------------------------------
	Path Path::parent()const {

		if (isRelative()) {
			return *this;
		}

		size_t offset = 0;
		if (m_string.ends_with(TC('/'))) {
			offset++;
		}

		auto pos = m_string.rfind(TC('/'), m_string.size() - offset);
		if (pos == m_string.npos) {
			return *this;
		}

		return m_string.substr(0, pos + 1);

	}

	//@―---------------------------------------------------------------------------
	//! @brief		正規化
	//@―---------------------------------------------------------------------------
	void Path::validate() {
		m_string.replace(TC('\\'), TC('/'));
	}

}