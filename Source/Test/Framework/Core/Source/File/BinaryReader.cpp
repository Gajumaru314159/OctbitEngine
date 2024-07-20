//***********************************************************
//! @file
//! @brief		BinaryReader のテストコード
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/File/BinaryReader.h>

using namespace ob::core;


TEST(BinaryReader, Construct) {
    Blob blob(1024);
    BlobStream stream(blob);
    BinaryReader reader(stream);
}

TEST(BinaryReader, Read) {

    struct TestStruct {
		s64 s64Value = 123;
		u64 u64Value = 123;
        s32 s32Value = 123;
		u32 u32Value = 123;
		s16 s16Value = 123;
		u16 u16Value = 123;
		s8  s8Value = 123;    
        u8  u8Value = 123;
        u8 pad[2];
		f64 f64Value = 123.0f;
		f32 f32Value = 123.0;
        Char text[5] = "test";
	} test;

    Blob blob(&test,sizeof(test));
    BlobStream stream(blob);
    BinaryReader reader(stream);

    EXPECT_EQ(reader.readS64(), 123);
    EXPECT_EQ(reader.readU64(), 123);
    EXPECT_EQ(reader.readS32(), 123);
    EXPECT_EQ(reader.readU32(), 123);
    EXPECT_EQ(reader.readS16(), 123);
    EXPECT_EQ(reader.readU16(), 123);
    EXPECT_EQ(reader.readS8(), 123);
    EXPECT_EQ(reader.readU8(), 123);
    reader.readU8();
    reader.readU8();
    EXPECT_EQ(reader.readF64(), 123.0f);
    EXPECT_EQ(reader.readF32(), 123.0f);

    Char text[5];
    reader.read(text, 5);

    EXPECT_TRUE(strcmp(text,test.text)==0);
}

TEST(BinaryReader, Seek) {

    struct TestStruct {
        s32 a = 123;
        s32 b = 456;
    } test;

    Blob blob(&test, sizeof(test));
    BlobStream stream(blob);
    BinaryReader reader(stream);

    EXPECT_EQ(reader.readS32(), 123);

    reader.seek(0,SeekOrigin::Begin);

    EXPECT_EQ(reader.readS32(), 123);

}

TEST(BinaryReader, ReadStruct) {

	struct TestStruct {
		s32 a = 123;
		s32 b = 456;
	} test;

	Blob blob(&test, sizeof(test));
	BlobStream stream(blob);
	BinaryReader reader(stream);

	TestStruct read;
	reader.read(read);

	EXPECT_EQ(read.a, 123);
	EXPECT_EQ(read.b, 456);
}

TEST(BinaryReader,IsEOF) {

    struct TestStruct {
		s32 a = 123;
		s32 b = 456;
	} test;

	Blob blob(&test, sizeof(test));
	BlobStream stream(blob);
	BinaryReader reader(stream);

	reader.readS32();
	reader.readS32();

	EXPECT_TRUE(reader.isEOF());

}