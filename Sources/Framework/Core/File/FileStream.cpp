//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/FileStream.h>
#include <Framework/Core/String/StringEncoder.h>
#include <stdio.h>
#include <filesystem>

#ifdef OS_WINDOWS
#include <sys/stat.h>
#include <sys/types.h>
#endif

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
		{
			auto wpath = path.wstring();
			if (_wfopen_s(&m_fp, wpath.c_str(), L"rb") == 0) {

			}

		}

		//@―---------------------------------------------------------------------------
		//! @brief  デストラクタ
		//@―---------------------------------------------------------------------------
		~FileStreamImpl() {
			flush();
		}

		//@―---------------------------------------------------------------------------
		//! @brief  読み込み可能か
		//@―---------------------------------------------------------------------------

		void checkOpen() {
			OB_CHECK_ASSERT_EXPR(m_fp);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  読み込み可能か
		//@―---------------------------------------------------------------------------
		bool canRead()const {
			return enum_cast(m_mode) & enum_cast(FileOpenMode::Read);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  書き込み可能か
		//@―---------------------------------------------------------------------------
		bool canWrite()const {
			return enum_cast(m_mode) & enum_cast(FileOpenMode::Write);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  ファイルサイズ取得
		//@―---------------------------------------------------------------------------
		size_t size()const {
			std::error_code code;
			auto s = file_size(m_path,code);
			if (s== static_cast<std::uintmax_t>(-1)) {
				return 0;
			}
			return (size_t)s;
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
		size_t read(void* buffer, size_t byteCount) {
			checkOpen();
			return fread_s(buffer, byteCount, sizeof(byte), byteCount, m_fp);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  書き込み
		//@―---------------------------------------------------------------------------
		size_t write(void* buffer, size_t byteCount) {
			checkOpen();
			return fwrite(buffer, sizeof(byte), byteCount, m_fp);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  シーク
		//@―---------------------------------------------------------------------------
		size_t seek(offset_t offset, SeekOrigin origin) {
			checkOpen();
			auto convert = [](SeekOrigin origin) {
				if (origin == SeekOrigin::Begin)return SEEK_SET;
				if (origin == SeekOrigin::Current)return SEEK_CUR;
				if (origin == SeekOrigin::End)return SEEK_END;
				return SEEK_SET;
			};

			if (fseek(m_fp, (long)offset, convert(origin))) {

			} else {
				// Failed
			}
			return position();
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
		FILE* m_fp=nullptr;
		FileOpenMode m_mode= FileOpenMode::None;
		Path m_path;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------
	FileStream::FileStream(const Path& path, FileOpenMode mode)
	{
		m_impl = std::make_unique<FileStreamImpl>(path, mode);
	}
	FileStream::~FileStream() = default;


	bool FileStream::canRead()const {
		return m_impl->canRead();
	}
	bool FileStream::canWrite()const {
		return m_impl->canWrite();
	}
	size_t FileStream::size()const {
		return m_impl->size();
	}
	size_t FileStream::position()const {
		return m_impl->position();
	}
	size_t FileStream::read(void* buffer, size_t byteCount) {
		return m_impl->read(buffer,byteCount);
	}
	size_t FileStream::write(void* buffer, size_t byteCount) {
		return m_impl->write(buffer,byteCount);
	}
	size_t FileStream::seek(offset_t offset, SeekOrigin origin) {
		return m_impl->seek(offset,origin);
	}
	void FileStream::flush() {
		return m_impl->flush();
	}
}// namespace ob