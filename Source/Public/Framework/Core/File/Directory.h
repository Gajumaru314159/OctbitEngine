//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Misc/DateTime.h>
#include <Framework/Core/Misc/YesNo.h>

namespace ob::core {

	DEFINE_YES_NO(Recrusive);

	enum class SearchOption :u32 {
		TopDirectoryOnly,
		Drecursive,
	};


	struct DirectoryInfo {};
	struct FileInfo {};

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class Directory {
	public:

		static DirectoryInfo CreateDirectory(Path path);
		static void Delete(Path path, Recrusive recrusive = Recrusive::Yes);

		using VisitDirectoryFunc = const std::function<void(const DirectoryInfo&)>&;
		using VisitFileFunc = const std::function<void(const FileInfo&)>&;

		static void VisitDirectories(Path path, VisitDirectoryFunc func);
		static void VisitFiles(Path path, VisitFileFunc func);
		//void VisitItems(Path path, VisitFileFunc func);
		//void VisitDirectories(Path path, VisitFunc func,Regex regex);
		static  bool Exists(Path path);
		static Path GetCurrentDirectory();
		static void Move(Path sourceDirName, Path destDirName);


		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------

	private:



	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond



	//! @endcond
}// namespcae ob