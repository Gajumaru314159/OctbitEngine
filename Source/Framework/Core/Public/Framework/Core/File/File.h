//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/File/Path.h>
#include <Framework/Core/Misc/DateTime.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  ファイル
	//@―---------------------------------------------------------------------------
	class File {
	public:

	private:

		 


	};


	enum class FileType {
		Directory,
		Reguler,
	};

	struct FileStatData {
		FileType type;
		DateTime creationTime;
		DateTime accessTime;
		DateTime modificationTime;
		size_t	 fileSize;

		bool bIsDirectory : 1;
		bool bIsReadOnly : 1;
		bool bIsValid : 1;
	};


	class IDirectoryVisitor {
	public:
		virtual ~IDirectoryVisitor() = default;

		virtual bool visit(Path path, const FileStatData& statData) = 0;

	};

	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond



	//! @endcond
}