//***********************************************************
//! @file
//! @brief		Hash のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Hash/Hash.h>

using namespace ob;

TEST(Hash, FNV32) {

    // 空文字列のハッシュ
    EXPECT_EQ(Hash::FNV32(TC("")), 0x811c9dc5);

    // 単一文字列のハッシュ
    EXPECT_EQ(Hash::FNV32(TC("a")), 0xe40c292c);
    EXPECT_EQ(Hash::FNV32(TC("b")), 0xe70c2de5);
    EXPECT_EQ(Hash::FNV32(TC("c")), 0xe60c2c52);

    // 数字のみの文字列のハッシュ
    EXPECT_EQ(Hash::FNV32(TC("123")), 0x7238631b);
    EXPECT_EQ(Hash::FNV32(TC("456")), 0x1e377b3c);
    EXPECT_EQ(Hash::FNV32(TC("789")), 0x91db9c09);

    // アルファベット文字列のハッシュ
    EXPECT_EQ(Hash::FNV32(TC("abcdefghijklmnopqrstuvwxyz")), 0xb0bc0c82);
    EXPECT_EQ(Hash::FNV32(TC("ABCDEFGHIJKLMNOPQRSTUVWXYZ")), 0x8a88dd82);

}

TEST(Hash, FNV64) {

    // 空文字列のハッシュ
    EXPECT_EQ(Hash::FNV64(TC("")), 0xcbf29ce484222325);

    // 単一文字列のハッシュ
    EXPECT_EQ(Hash::FNV64(TC("a")), 0xaf63dc4c8601ec8c);
    EXPECT_EQ(Hash::FNV64(TC("b")), 0xaf63df4c8601f1a5);
    EXPECT_EQ(Hash::FNV64(TC("c")), 0xaf63de4c8601eff2);

    // 数字のみの文字列のハッシュ
    EXPECT_EQ(Hash::FNV64(TC("123")), 0x456fc2181822c4db);
    EXPECT_EQ(Hash::FNV64(TC("456")), 0x2be33a1809c9a97c);
    EXPECT_EQ(Hash::FNV64(TC("789")), 0x350a6a180f1c6dc9);

    // アルファベット文字列のハッシュ
    EXPECT_EQ(Hash::FNV64(TC("abcdefghijklmnopqrstuvwxyz")), 0x8450deb1cdc382a2);
    EXPECT_EQ(Hash::FNV64(TC("ABCDEFGHIJKLMNOPQRSTUVWXYZ")), 0xe276b7953364d122);

}

TEST(Hash, FNV32Const) {

    // 空文字列のハッシュ
    EXPECT_EQ(OB_FNV32(""), 0x811c9dc5);

    // 単一文字列のハッシュ
    EXPECT_EQ(OB_FNV32("a"), 0xe40c292c);
    EXPECT_EQ(OB_FNV32("b"), 0xe70c2de5);
    EXPECT_EQ(OB_FNV32("c"), 0xe60c2c52);

    // 数字のみの文字列のハッシュ
    EXPECT_EQ(OB_FNV32("123"), 0x7238631b);
    EXPECT_EQ(OB_FNV32("456"), 0x1e377b3c);
    EXPECT_EQ(OB_FNV32("789"), 0x91db9c09);

    // アルファベット文字列のハッシュ
    EXPECT_EQ(OB_FNV32("abcdefghijklmnopqrstuvwxyz"), 0xb0bc0c82);
    EXPECT_EQ(OB_FNV32("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), 0x8a88dd82);

}

TEST(Hash, FNV64Const) {

    // 空文字列のハッシュ
    EXPECT_EQ(OB_FNV64(""), 0xcbf29ce484222325);

    // 単一文字列のハッシュ
    EXPECT_EQ(OB_FNV64("a"), 0xaf63dc4c8601ec8c);
    EXPECT_EQ(OB_FNV64("b"), 0xaf63df4c8601f1a5);
    EXPECT_EQ(OB_FNV64("c"), 0xaf63de4c8601eff2);

    // 数字のみの文字列のハッシュ
    EXPECT_EQ(OB_FNV64("123"), 0x456fc2181822c4db);
    EXPECT_EQ(OB_FNV64("456"), 0x2be33a1809c9a97c);
    EXPECT_EQ(OB_FNV64("789"), 0x350a6a180f1c6dc9);

    // アルファベット文字列のハッシュ
    EXPECT_EQ(OB_FNV64("abcdefghijklmnopqrstuvwxyz"), 0x8450deb1cdc382a2);
    EXPECT_EQ(OB_FNV64("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), 0xe276b7953364d122);

}