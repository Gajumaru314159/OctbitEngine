#include <gtest/gtest.h>
#include <Framework/Core/File/Directory.h>
#include <Framework/Core/File/File.h>

using namespace ob::core;

TEST(DirectoryTest, Exists)
{
    

    auto path = "unit_test";

    Directory::Create(path);

    // ディレクトリが存在するかテスト
    ASSERT_TRUE(Directory::Exists(path));


    Directory::Delete(path);

    // ディレクトリが存在しないかテスト
    ASSERT_FALSE(Directory::Exists(path));
}

TEST(DirectoryTest, Empty)
{
    
    auto path = "unit_test";

    Directory::Delete(path);
    Directory::Create(path);

    // 空のディレクトリかテスト
    ASSERT_TRUE(Directory::Empty(path));

    File file(Format("{}/file.txt",path), FileOpenMode::Write);
    file.close();


    // 空でないディレクトリかテスト
    ASSERT_FALSE(Directory::Empty(path));

    Directory::Delete(path);
}

TEST(DirectoryTest, Current)
{
    
    auto path = "unit_test";

    auto current = Directory::Current();

    Directory::Create(path);

    Directory::ChangeCurrentDirectory(path);

    EXPECT_FALSE(Directory::Current() == current);

    Directory::ChangeCurrentDirectory(current);

    EXPECT_TRUE(Directory::Current() == current);

    Directory::Delete(path);
}

TEST(DirectoryTest, Create)
{
    
    auto path = "unit_test";

    // ディレクトリの作成をテスト
    Directory::Create(path);

    // ディレクトリが存在することを確認
    ASSERT_TRUE(Directory::Exists(path));

    Directory::Delete(path);
}

TEST(DirectoryTest, Move)
{
    
    auto path = "unit_test";
    auto path2 = "unit_test2";

    Directory::Delete(path);
    Directory::Delete(path2);
    Directory::Create(path);

    // ディレクトリの移動をテスト
    Directory::Move(path,path2);

    // 元のディレクトリが存在しないことを確認
    ASSERT_FALSE(Directory::Exists(path));

    // 移動先のディレクトリが存在することを確認
    ASSERT_TRUE(Directory::Exists(path2));

    Directory::Delete(path2);
}

TEST(DirectoryTest, Rename)
{
    
    auto path = "unit_test";
    auto path2 = "unit_test2";

    Directory::Delete(path);
    Directory::Delete(path2);
    Directory::Create(path);

    // ディレクトリの移動をテスト
    Directory::Rename(path, path2);

    // 元のディレクトリが存在しないことを確認
    ASSERT_FALSE(Directory::Exists(path));

    // 移動先のディレクトリが存在することを確認
    ASSERT_TRUE(Directory::Exists(path2));

    Directory::Delete(path2);
}

TEST(DirectoryTest, Copy)
{
    
    auto path = "unit_test";
    auto path2 = "unit_test2";

    Directory::Delete(path);
    Directory::Delete(path2);
    Directory::Create(path);

    // ディレクトリのコピーをテスト
    Directory::Copy(path,path2);

    // 元のディレクトリが存在することを確認
    ASSERT_TRUE(Directory::Exists(path));

    // コピー先のディレクトリが存在することを確認
    ASSERT_TRUE(Directory::Exists(path2));
}

TEST(DirectoryTest, Delete)
{
    
    auto path = "unit_test";
    // ディレクトリの削除をテスト
    Directory::Create(path);
    Directory::Delete(path);

    // ディレクトリが存在しないことを確認
    ASSERT_FALSE(Directory::Exists(path));
}

TEST(DirectoryTest, DeleteContents)
{
    
    auto path = "unit_test";

    Directory::Create(path);

    for (s32 i = 0; i < 10; ++i) {
        File file(Format("{}/file{}.txt", path,i), FileOpenMode::Write);
    }

    // ディレクトリの内容の削除をテスト
    Directory::DeleteContents(path);

    // ディレクトリが空であることを確認
    ASSERT_TRUE(Directory::Exists(path));
    ASSERT_TRUE(Directory::Empty(path));
    Directory::Delete(path);
}

TEST(DirectoryTest, Contents)
{
    
    auto path = "unit_test";

    Directory::Delete(path);
    Directory::Create(path);

    for (s32 i = 0; i < 10; ++i) {
        File file(Format("{}/file{}.txt", path, i), FileOpenMode::Write);
    }

    // ディレクトリの内容を取得するテスト
    auto contents = Directory::Contents(path);

    // 内容のサイズを確認
    ASSERT_EQ(contents.size(), 10);

    Directory::Delete(path);
}
