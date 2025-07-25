//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/Directory.h>
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

    //! @brief  ディレクトリが存在するか
    //! @details ファイルの存在を確認する場合は File::Exists を使用してください。
    bool Directory::Exists(StringView path) {
        if (!std::filesystem::is_directory(ToStdPath(path)))return false;
        return std::filesystem::exists(ToStdPath(path));
    }

    //! @brief  ディレクトリが空か
    bool Directory::Empty(StringView path) {
        return std::filesystem::is_empty(ToStdPath(path));
    }

    //! @brief  カレントディレクトリを取得
    String Directory::Current() {
        return ToString(std::filesystem::current_path());
    }

    //! @brief  カレントディレクトリを変更
    void Directory::ChangeCurrentDirectory(StringView path) {
        std::filesystem::current_path(ToStdPath(path));
    }

    //! @brief  ディレクトリ作成
    //! @details 間のディレクトリが存在しない場合は自動的に作成されます。
    void Directory::Create(StringView path) {
        std::filesystem::create_directories(ToStdPath(path));
    }

    //! @brief  ディレクトリ移動
    void Directory::Move(StringView from, StringView to) {
        std::filesystem::rename(ToStdPath(from), ToStdPath(to));
    }

    //! @brief  ディレクトリ名変更
    void Directory::Rename(StringView from, StringView to) {
        std::filesystem::rename(ToStdPath(from), ToStdPath(to));
    }

    //! @brief  ディレクトリコピー
    void Directory::Copy(StringView from, StringView to) {
        std::filesystem::copy(ToStdPath(from), ToStdPath(to));
    }

    //! @brief  ディレクトリ削除
    void Directory::Delete(StringView path) {
        std::filesystem::remove_all(ToStdPath(path));
    }

    //! @brief  ディレクトリ内容削除
    //! @details pathに指定したディレクトリ自体は削除されません。
    void Directory::DeleteContents(StringView path) {
        for (auto& p : std::filesystem::directory_iterator(ToStdPath(path))) {
            std::filesystem::remove_all(p);
        }
    }

    //! @brief  ディレクトリ内容取得
    auto Directory::Contents(StringView path, Recursive recursive) -> Vector<String> {
        Vector<String> result;
        if (recursive) {
            for (auto& p : std::filesystem::directory_iterator(ToStdPath(path))) {
                result.push_back(ToString(p.path()));
            }
        }
        else {
            for (auto& p : std::filesystem::recursive_directory_iterator(ToStdPath(path))) {
                result.push_back(ToString(p.path()));
            }
        }
        return result;
    }

}