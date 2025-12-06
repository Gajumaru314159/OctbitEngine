//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/File/File.h>

using namespace ob::core;

TEST(FileTest, Exists)
{
    
    File file("test.txt", FileOpenMode::Write);
    file.close();

    // ファイルが存在する場合
    EXPECT_TRUE(File::Exists("test.txt"));

    File::Delete("test.txt");

    // ファイルが存在しない場合
    EXPECT_FALSE(File::Exists("test.txt"));
}

TEST(FileTest, Size)
{
    
    Blob blob(1024);
    File file("test.txt",FileOpenMode::Write);
    file.write(blob.data(),blob.size());
    file.close();

    // ファイルのサイズを取得
    size_t fileSize = File::Size("test.txt");

    // サイズが正しいことを確認
    EXPECT_EQ(fileSize, 1024);

    File::Delete("test.txt");
}

TEST(FileTest, Copy)
{
    
    File file("test.txt", FileOpenMode::Write);
    file.close();

    // ファイルをコピー
    bool success = File::Copy("test.txt","test_copy.txt");

    // コピーが成功したことを確認
    EXPECT_TRUE(success);
    EXPECT_TRUE(File::Exists("test.txt"));
    EXPECT_TRUE(File::Exists("test_copy.txt"));

    File::Delete("test.txt");
    File::Delete("test_copy.txt");
}

TEST(FileTest, Delete)
{
    
    File file("test.txt", FileOpenMode::Write);
    file.close();

    // ファイルを削除
    bool success = File::Delete("test.txt");

    // 削除が成功したことを確認
    EXPECT_TRUE(success);
    EXPECT_FALSE(File::Exists("test.txt"));
}

TEST(FileTest, Move)
{
    
    File::Delete("test_move.txt");
    File file("test.txt", FileOpenMode::Write);
    file.close();
 
    // ファイルを移動
    bool success = File::Move("test.txt", "test_move.txt");

    // 移動が成功したことを確認
    EXPECT_TRUE(success);
    EXPECT_FALSE(File::Exists("test.txt"));
    EXPECT_TRUE(File::Exists("test_move.txt"));

    File::Delete("test_move.txt");
}

TEST(FileTest, Rename)
{
    
    File::Delete("test_rename.txt");
    File file("test.txt", FileOpenMode::Write);
    file.close();

    // ファイルを移動
    bool success = File::Rename("test.txt", "test_rename.txt");

    // 移動が成功したことを確認
    EXPECT_TRUE(success);
    EXPECT_FALSE(File::Exists("test.txt"));
    EXPECT_TRUE(File::Exists("test_rename.txt"));

    File::Delete("test_rename.txt");
}

TEST(FileTest, ReadAllText)
{
    
    String text = "abcdefg";
    File file("test.txt", FileOpenMode::Write);
    file.write(text.data(), text.size());
    file.close();

    // ファイルをテキストとして読み込む
    Optional<String> text2 = File::ReadAllText("test.txt");

    // テキストが正しく読み込まれたことを確認
    EXPECT_TRUE(text2);
    EXPECT_EQ(*text2, "abcdefg");
    
    File::Delete("test.txt");
}