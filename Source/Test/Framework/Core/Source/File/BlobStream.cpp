//***********************************************************
//! @file
//! @brief		BlobStream のテストコード
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/File/BlobStream.h>

using namespace ob::core;

TEST(BlobStream, CanRead)
{
    Blob blob(1024);
    BlobStream stream(blob);

    // 読み込み可能かを確認
    EXPECT_TRUE(stream.canRead());
}

TEST(BlobStream, CanWrite)
{
    Blob blob(1024);
    BlobStream stream(blob);

    // 書き込み可能かを確認
    EXPECT_TRUE(stream.canWrite());
}

TEST(BlobStream, Size)
{
    Blob blob(1024);
    BlobStream stream(blob);

    // サイズを取得
    size_t streamSize = stream.size();

    // サイズが正しいことを確認
    EXPECT_EQ(streamSize, 1024);
}

TEST(BlobStream, Position)
{
    Blob blob(1024);
    BlobStream stream(blob);

    // シーク位置を取得
    size_t streamPosition = stream.position();

    // シーク位置が正しいことを確認
    EXPECT_EQ(streamPosition, 0);
}

TEST(BlobStream, Read)
{
    Blob blob(1024);
    BlobStream stream(blob);

    char buffer[256];

    // バッファにデータを読み込む
    bool success = stream.read(buffer, sizeof(buffer));

    // 読み込みが成功したことを確認
    EXPECT_TRUE(success);
}

TEST(BlobStream, Write)
{
    Blob blob(1024);
    BlobStream stream(blob);

    char buffer[256];

    // バッファのデータを書き込む
    bool success = stream.write(buffer, sizeof(buffer));

    // 書き込みが成功したことを確認
    EXPECT_TRUE(success);
}

TEST(BlobStream, Seek)
{
    Blob blob(1024);
    BlobStream stream(blob);

    // シークする
    bool success = stream.seek(256, SeekOrigin::Begin);

    // シークが成功したことを確認
    EXPECT_TRUE(success);
}

TEST(BlobStream, Flush)
{
    Blob blob(1024);
    BlobStream stream(blob);

    // バッファをフラッシュする
    stream.flush();
}
