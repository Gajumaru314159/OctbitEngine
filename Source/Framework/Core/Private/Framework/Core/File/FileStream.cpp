﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/FileStream.h>
#include <Framework/Core/String/StringEncoder.h>
#include <stdio.h>
#include <filesystem>

namespace ob::core {

	class FileStreamImpl {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		FileStreamImpl(const Path& path, FileOpenMode mode)
			: m_fp(nullptr)
			, m_mode(mode)
			, m_path(path)
			, m_size(0)
		{
			const wchar_t* pMode = L"";
			switch (mode) {
			case ob::core::FileOpenMode::Read:		pMode = L"rb"; break;
			case ob::core::FileOpenMode::Write:		pMode = L"wb"; break;
			case ob::core::FileOpenMode::Append:	pMode = L"ab"; break;
			default:LOG_WARNING("不正なFileOpenMode[value={}]", enum_cast(mode)); break;
			}
			auto wpath = path.wstring();
			if (_wfopen_s(&m_fp, wpath.c_str(), pMode) == 0) {
				std::error_code code;
				auto s = file_size(m_path, code);
				if (s != static_cast<std::uintmax_t>(-1)) {
					m_size = (size_t)s;
				} else {
					String path2;
					StringEncoder::Encode(path.wstring(), path2);
					LOG_WARNING("サイズの取得に失敗[{}]", path2);
					m_fp = nullptr;
				}
			} else {
				String path2;
				StringEncoder::Encode(path.wstring(), path2);
				LOG_WARNING("ファイルを開けませんでした[{}]", path2);
				m_fp = nullptr;
			}

		}

		//@―---------------------------------------------------------------------------
		//! @brief  デストラクタ
		//@―---------------------------------------------------------------------------
		~FileStreamImpl() {
			flush();
		}

		//@―---------------------------------------------------------------------------
		//! @brief  有効な状態か
		//@―---------------------------------------------------------------------------
		bool isValid()const {
			return m_fp != nullptr;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  ファイルオープンチェック
		//@―---------------------------------------------------------------------------
		void checkOpen() {
			OB_ASSERT_EXPR(m_fp);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  読み込み可能か
		//@―---------------------------------------------------------------------------
		bool canRead()const {
			if (!isValid())return false;
			return enum_cast(m_mode) & enum_cast(FileOpenMode::Read);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  書き込み可能か
		//@―---------------------------------------------------------------------------
		bool canWrite()const {
			if (!isValid())return false;
			return enum_cast(m_mode) & enum_cast(FileOpenMode::Write);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  ファイルサイズ取得
		//@―---------------------------------------------------------------------------
		size_t size()const {
			return m_size;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  読み取り位置取得
		//@―---------------------------------------------------------------------------
		size_t position()const {
			fpos_t pos;
			if (m_fp == nullptr)return 0;
			if (fgetpos(m_fp, &pos)) {
				return 0;
			}
			return (size_t)pos;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  読み取り
		//@―---------------------------------------------------------------------------
		bool read(void* buffer, size_t byteCount) {
			checkOpen();
			offset_t readCount = fread_s(buffer, byteCount, sizeof(byte), byteCount, m_fp);
			if(readCount!=byteCount) {
				// readCount 読めなかったら戻す
				seek(-readCount, SeekOrigin::Current);
				return false;
			}
			return true;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  書き込み
		//@―---------------------------------------------------------------------------
		bool write(const void* buffer, size_t byteCount) {
			checkOpen();
			offset_t writeCount = fwrite(buffer, sizeof(byte), byteCount, m_fp);
			m_size = std::max(m_size,position()+writeCount);
			if (writeCount != byteCount) {
				String path;
				StringEncoder::Encode(m_path.wstring(), path);
				LOG_ERROR("ファイルの書き込みに失敗[{}]", path);
				return false;
			}
			return true;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  シーク
		//@―---------------------------------------------------------------------------
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
				String path;
				StringEncoder::Encode(m_path.wstring(), path);
				LOG_ERROR("ファイルのシークに失敗[{}]", path);
				return false;
			}
			return true;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  フラッシュ
		//@―---------------------------------------------------------------------------
		void flush() {
			if (m_fp) {
				fflush(m_fp);
			}
		}

	private:
		FILE* m_fp = nullptr;
		FileOpenMode m_mode;
		Path m_path;
		size_t m_size;
	};



	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	FileStream::FileStream(const Path& path, FileOpenMode mode) :m_impl(path, mode) {}
	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	FileStream::~FileStream() = default;
	//@―---------------------------------------------------------------------------
	//! @brief  有効な状態か
	//@―---------------------------------------------------------------------------
	FileStream::operator bool()const { return m_impl->isValid(); }
	//@―---------------------------------------------------------------------------
	//! @brief  読み込み可能か
	//@―---------------------------------------------------------------------------
	bool FileStream::canRead()const { return m_impl->canRead(); }
	//@―---------------------------------------------------------------------------
	//! @brief  書き込み可能か
	//@―---------------------------------------------------------------------------
	bool FileStream::canWrite()const { return m_impl->canWrite(); }
	//@―---------------------------------------------------------------------------
	//! @brief  ファイルサイズ取得
	//@―---------------------------------------------------------------------------
	size_t FileStream::size()const { return m_impl->size(); }
	//@―---------------------------------------------------------------------------
	//! @brief  読み込み位置取得
	//@―---------------------------------------------------------------------------
	size_t FileStream::position()const { return m_impl->position(); }
	//@―---------------------------------------------------------------------------
	//! @brief  読み込み
	//@―---------------------------------------------------------------------------
	bool FileStream::read(void* buffer, size_t byteCount) { return m_impl->read(buffer, byteCount); }
	//@―---------------------------------------------------------------------------
	//! @brief  書き込み
	//@―---------------------------------------------------------------------------
	bool FileStream::write(const void* buffer, size_t byteCount) { return m_impl->write(buffer, byteCount); }
	//@―---------------------------------------------------------------------------
	//! @brief  シーク
	//@―---------------------------------------------------------------------------
	bool FileStream::seek(offset_t offset, SeekOrigin origin) { return m_impl->seek(offset, origin); }
	//@―---------------------------------------------------------------------------
	//! @brief  フラッシュ
	//@―---------------------------------------------------------------------------
	void FileStream::flush() { return m_impl->flush(); }

}// namespace ob