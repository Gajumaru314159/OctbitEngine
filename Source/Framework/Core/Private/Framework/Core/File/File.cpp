//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/File.h>
#include <filesystem>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  ファイルが存在するか
	//@―---------------------------------------------------------------------------
	bool File::Exists(StringView path) {
		return std::filesystem::is_regular_file((std::string_view)path);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ファイルサイズを取得
	//@―---------------------------------------------------------------------------
	size_t File::Size(StringView path) {
		return std::filesystem::file_size((std::string_view)path);
	}

}