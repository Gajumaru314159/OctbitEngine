//***********************************************************
//! @file
//! @brief		BinaryWriter のテストコード
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/File/BinaryWriter.h>

using namespace ob::core;

TEST(BinaryWriter, Constructor)
{
    Blob blob;
    BlobStream stream(blob);
    BinaryWriter writer(stream);
}

TEST(BinaryWriter, Write) {

	Blob blob;
	BlobStream stream(blob);
	BinaryWriter writer(stream);

	writer.writeInt64(123);
	writer.writeUInt64(123);
	writer.writeInt32(123);
	writer.writeUInt32(123);
	writer.writeInt16(123);
	writer.writeUInt16(123);
	writer.writeInt8(123);
	writer.writeUInt8(123);
	writer.writeDouble(123.0f);
	writer.writeFloat(123.0f);

	EXPECT_EQ(blob.size(), 42);
}