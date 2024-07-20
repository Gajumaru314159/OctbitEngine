//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/Directory.h>
#include <filesystem>


namespace ob::core {


    //@―---------------------------------------------------------------------------
    //! @brief  ディレクトリが存在するか
    //! @details ファイルの存在を確認する場合は File::Exists を使用してください。
    //@―---------------------------------------------------------------------------
    bool Directory::Exists(StringView path) {
        if (!std::filesystem::is_directory((std::string_view)path))return false;
        return std::filesystem::exists((std::string_view)path);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ディレクトリが空か
    //@―---------------------------------------------------------------------------
    bool Directory::Empty(StringView path) {
        return std::filesystem::is_empty((std::string_view)path);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  カレントディレクトリを取得
    //@―---------------------------------------------------------------------------
    String Directory::Current() {
        return std::filesystem::current_path().string();
    }

    //@―---------------------------------------------------------------------------
    //! @brief  カレントディレクトリを変更
    //@―---------------------------------------------------------------------------
    void Directory::ChangeCurrentDirectory(StringView path) {
        std::filesystem::current_path((std::string_view)path);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ディレクトリ作成
    //! @details 間のディレクトリが存在しない場合は自動的に作成されます。
    //@―---------------------------------------------------------------------------
    void Directory::Create(StringView path) {
        std::filesystem::create_directories((std::string_view)path);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ディレクトリ移動
    //@―---------------------------------------------------------------------------
    void Directory::Move(StringView from, StringView to) {
        std::filesystem::rename((std::string_view)from, (std::string_view)to);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ディレクトリ名変更
    //@―---------------------------------------------------------------------------
    void Directory::Rename(StringView from, StringView to) {
        std::filesystem::rename((std::string_view)from, (std::string_view)to);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ディレクトリコピー
    //@―---------------------------------------------------------------------------
    void Directory::Copy(StringView from, StringView to) {
        std::filesystem::copy((std::string_view)from, (std::string_view)to);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ディレクトリ削除
    //@―---------------------------------------------------------------------------
    void Directory::Delete(StringView path) {
        std::filesystem::remove_all((std::string_view)path);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ディレクトリ内容削除
    //! @details pathに指定したディレクトリ自体は削除されません。
    //@―---------------------------------------------------------------------------
    void Directory::DeleteContents(StringView path) {
        for (auto& p : std::filesystem::directory_iterator((std::string_view)path)) {
            std::filesystem::remove_all(p);
        }
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ディレクトリ内容取得
    //@―---------------------------------------------------------------------------
    auto Directory::Contents(StringView path, Recursive recursive) -> Vector<String> {
        Vector<String> result;
        if (recursive) {
            for (auto& p : std::filesystem::directory_iterator((std::string_view)path)) {
                result.push_back(p.path().string());
            }
        }
        else {
            for (auto& p : std::filesystem::recursive_directory_iterator((std::string_view)path)) {
                result.push_back(p.path().string());
            }
        }
        return result;
    }

}