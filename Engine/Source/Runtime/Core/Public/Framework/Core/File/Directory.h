//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Misc/YesNo.h>

namespace ob::core {

    //! @brief  再帰的か
    DEFINE_YES_NO(Recursive);

    //! @brief  ディレクトリ
    class Directory {
    public:

        //! @brief  ディレクトリが存在するか
        //! @details ファイルの存在を確認する場合は File::Exists を使用してください。
        static bool Exists(StringView path);

        //! @brief  ディレクトリが空か
        static bool Empty(StringView path);

        //! @brief  カレントディレクトリを取得
        static String Current();

        //! @brief  カレントディレクトリを変更
        static void ChangeCurrentDirectory(StringView path);

        //! @brief  ディレクトリ作成
        //! @details 間のディレクトリが存在しない場合は自動的に作成されます。
        static void Create(StringView path);

        //! @brief  ディレクトリ移動
        static void Move(StringView from, StringView to);

        //! @brief  ディレクトリ名変更
        static void Rename(StringView from, StringView to);

        //! @brief  ディレクトリコピー
        static void Copy(StringView from, StringView to);

        //! @brief  ディレクトリ削除
        static void Delete(StringView path);

        //! @brief  ディレクトリ内容削除
        //! @details pathに指定したディレクトリ自体は削除されません。
        static void DeleteContents(StringView path);

        //! @brief  ディレクトリ内容取得
        static auto Contents(StringView path, Recursive recursive = Recursive::No) -> Vector<String>;
    };

}