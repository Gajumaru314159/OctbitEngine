//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class File {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------
		size_t pos();
		bool seek(size_t pos);
		bool seekFromEnd(size_t offset);
		bool read(byte* dest, size_t size);
		bool write(const byte* dest, size_t size);
		bool flush();
		bool truncate(size_t size);
		size_t size();


	private:



	};


	enum class FileType {
		Directory,
		Reguler,
	};

	struct FileStatData {
		FileType type;
		Time creationTime;
		Time accessTime;
		Time modificationTime;
		size_t fileSize;

		bool bIsDirectory : 1;
		bool bIsReadOnly : 1;
		bool bIsValid : 1;
	};


	class IDirectoryVisitor {
	public:
		virtual ~IDirectoryVisitor() = default;

		virtual bool visit(Path path, const FileStatData& statData) = 0;

	}

	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond



	//! @endcond
}// namespcae ob