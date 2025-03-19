//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/File.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/Core/String/StringEncoder.h>
#include <filesystem>


namespace ob::core {

	using namespace std::filesystem;

	//! @brief		std::filesystem::pathに変換
	static std::filesystem::path ToStdPath(StringView path) {
		return std::filesystem::u8path((std::string_view)path);
	}

	//! @brief  エラーメッセージを取得
	static String GetErrnoString() {
		std::string errorStr = strerror(errno);
		String errorStr2;
		StringEncoder::Encode(errorStr, errorStr2);
		return errorStr2;
	}

	//! @brief  ファイル(Windows)
	class FileImpl {
	public:

		//! @brief  コンストラクタ
		FileImpl() {

		}

		//! @brief  コンストラクタ
		FileImpl(StringView path, FileOpenMode mode)
			: m_fp(nullptr)
			, m_mode(mode)
			, m_path(path)
			, m_size(0)
		{
			BitFlags modes(mode);
			const wchar_t* pMode = L"";
			if (modes.has(FileOpenMode::Text)) {
				if (modes.has(FileOpenMode::Read))pMode = L"r";
				if (modes.has(FileOpenMode::Write))pMode = L"w";
				if (modes.has(FileOpenMode::Append))pMode = L"a";
			}
			else {
				if (modes.has(FileOpenMode::Read))pMode = L"rb";
				if (modes.has(FileOpenMode::Write))pMode = L"wb";
				if (modes.has(FileOpenMode::Append))pMode = L"ab";
			}

			WString wpath;
			StringEncoder::Encode(path, wpath);
			auto err = _wfopen_s(&m_fp, wpath.c_str(), pMode);
			if (err == 0) {
				std::error_code code;
				std::filesystem::path fspath = wpath.c_str();
				auto s = file_size(fspath, code);
				if (s != static_cast<std::uintmax_t>(-1)) {
					m_size = (size_t)s;
				}
				else {
					LOG_WARNING("サイズの取得に失敗[{}]\n{}", path, GetErrnoString());
					close();
				}
			}
			else {
				LOG_WARNING("ファイルを開けませんでした[{}]\n{}", path, GetErrnoString());
				if (modes.has(FileOpenMode::Read) && !File::Exists(path)) {
					LOG_WARNING("{}は存在しません",path);
				}
				close();
			}

		}

		//! @brief  デストラクタ
		~FileImpl() {
			flush();
			close();
		}

		//! @brief  クローズ
		void close() {
			if (m_fp) {
				fclose(m_fp);
				m_fp = nullptr;
			}
		}

		//! @brief  有効な状態か
		bool isValid()const {
			return m_fp != nullptr;
		}

		//! @brief  ファイルオープンチェック
		void checkOpen() {
			OB_ASSERT_EXPR(m_fp);
		}

		//! @brief  読み込み可能か
		bool canRead()const {
			if (!isValid())return false;
			return enum_cast(m_mode) & enum_cast(FileOpenMode::Read);
		}

		//! @brief  書き込み可能か
		bool canWrite()const {
			if (!isValid())return false;
			return enum_cast(m_mode) & enum_cast(FileOpenMode::Write);
		}

		//! @brief  ファイルサイズ取得
		size_t size()const {
			return m_size;
		}

		//! @brief  読み取り位置取得
		size_t position()const {
			fpos_t pos;
			if (m_fp == nullptr)return 0;
			if (fgetpos(m_fp, &pos)) {
				LOG_WARNING("読み取り位置の取得に失敗[{}]\n{}", m_path, GetErrnoString());
				return 0;
			}
			return (size_t)pos;
		}

		//! @brief  読み取り
		bool read(void* buffer, size_t byteCount) {
			checkOpen();
			offset_t readCount = fread_s(buffer, byteCount, sizeof(byte), byteCount, m_fp);
			if (readCount != byteCount) {
				LOG_WARNING("読み取り失敗[{}]\n{}", m_path, GetErrnoString());
				// readCount 読めなかったら戻す
				seek(-readCount, SeekOrigin::Current);
				return false;
			}
			return true;
		}

		//! @brief  書き込み
		bool write(const void* buffer, size_t byteCount) {
			checkOpen();
			offset_t writeCount = fwrite(buffer, sizeof(byte), byteCount, m_fp);
			m_size = std::max(m_size, position() + writeCount);
			if (writeCount != byteCount) {
				LOG_WARNING("書き込みに失敗[{}]\n{}", m_path, GetErrnoString());
				return false;
			}
			return true;
		}

		//! @brief  シーク
		bool seek(offset_t offset, SeekOrigin origin) {
			checkOpen();

			auto convert = [](SeekOrigin origin) {
				if (origin == SeekOrigin::Begin)return SEEK_SET;
				if (origin == SeekOrigin::Current)return SEEK_CUR;
				if (origin == SeekOrigin::End)return SEEK_END;
				return SEEK_SET;
				};

			// fseekにSEEK_ENDを指定すると結果が不定になるため先頭からに変換
			if (origin == SeekOrigin::End) {
				origin = SeekOrigin::Begin;
				offset += size();
			}

			if (fseek(m_fp, (long)offset, convert(origin)) != 0) {
				LOG_WARNING("ファイルのシークに失敗[{}]\n{}", m_path, GetErrnoString());

				return false;
			}
			return true;
		}

		//! @brief  フラッシュ
		void flush() {
			if (m_fp) {
				if (fflush(m_fp) == EOF) {
					LOG_WARNING("ファイルのフラッシュに失敗[{}]\n{}", m_path, GetErrnoString());
				}
			}
		}

	private:
		FILE* m_fp = nullptr;
		FileOpenMode m_mode;
		String m_path;
		size_t m_size;
	};





	//! @brief  ファイルが存在するか
	bool File::Exists(StringView path) {
		return is_regular_file(ToStdPath(path));
	}

	//! @brief  ファイルサイズを取得
	size_t File::Size(StringView path) {
		return file_size(ToStdPath(path));
	}

	//! @brief  ファイルをコピーする
	//! @param src		コピー元
	//! @param dst		コピー先
	//! @param options	オプション
	//! @return 成功したか
	bool File::Copy(StringView src, StringView dst, FileCopyOptions options) {
		copy_options opt = copy_options::none;
		if (options.has(FileCopyOption::SkipExisting)) opt |= copy_options::skip_existing;
		if (options.has(FileCopyOption::OverwriteExisting)) opt |= copy_options::overwrite_existing;
		if (options.has(FileCopyOption::UpdateExisting)) opt |= copy_options::update_existing;

		return copy_file(ToStdPath(src), ToStdPath(dst), opt);
	}

	//! @brief	ファイルを削除する
	bool File::Delete(StringView path) {
		if (!Exists(path)) return false;
		return remove(ToStdPath(path));
	}

	//! @brief  ファイルを移動する
	bool File::Move(StringView from, StringView to) {
		std::error_code ec;
		rename(ToStdPath(from), ToStdPath(to), ec);
		return !ec.operator bool();
	}

	//! @brief  ファイル名を変更する
	bool File::Rename(StringView from, StringView to) {
		return Move(from, to);
	}

	//! @brief  ファイルを文字列として全て読み込む
	Optional<String> File::ReadAllText(StringView path) {
		File file(path);
		if (!file.canRead())return std::nullopt;

		String result;
		result.resize(file.size());
		file.read(result.data(), result.size());

		return result;
	}

	//! @brief  ファイルをバイナリデータとして全て読み込む
	Optional<Blob> File::ReadAllByte(StringView path) {
		File file(path);
		if (!file.canRead())return std::nullopt;

		Blob result;
		result.resize(file.size());
		file.read(result.data(), result.size());

		return result;
	}


	//! @brief  デフォルトコンストラクタ
	File::File() {}
	//! @brief  コンストラクタ
	File::File(StringView path, FileOpenMode mode) :m_impl(std::make_unique<FileImpl>(path, mode)) {}
	//! @brief  デストラクタ
	File::~File() = default;
	//! @brief  有効な状態か
	File::operator bool()const { return m_impl->isValid(); }
	//! @brief  読み込み可能か
	bool File::canRead()const { return m_impl->canRead(); }
	//! @brief  書き込み可能か
	bool File::canWrite()const { return m_impl->canWrite(); }
	//! @brief  ファイルサイズ取得
	size_t File::size()const { return m_impl->size(); }
	//! @brief  読み込み位置取得
	size_t File::position()const { return m_impl->position(); }
	//! @brief  読み込み
	bool File::read(void* buffer, size_t byteCount) { return m_impl->read(buffer, byteCount); }
	//! @brief  書き込み
	bool File::write(const void* buffer, size_t byteCount) { return m_impl->write(buffer, byteCount); }
	//! @brief  シーク
	bool File::seek(offset_t offset, SeekOrigin origin) { return m_impl->seek(offset, origin); }
	//! @brief  フラッシュ
	void File::flush() { return m_impl->flush(); }
	//! @brief  ファイルをクローズして無効なハンドルにする
	void File::close() { return m_impl->close(); }
}