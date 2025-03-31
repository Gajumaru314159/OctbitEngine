//***********************************************************
//! @file
//! @brief		AsyncFile のテストコード
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/File/AsyncFile.h>

using namespace ob::core;

TEST(AsyncFile, AsyncFileA)
{
	AsyncFileSystem system;

	{
		File file("sample.bin", FileOpenMode::Write);
		for (s32 i = 0; i < 100; ++i) {
			file.write(&i, sizeof(i));
		}
	}

	{
		AsyncFileHandle handle("sample.bin");

		EXPECT_EQ(handle.path(), "sample.bin");
		EXPECT_FALSE(handle.empty());
		EXPECT_EQ(handle.size(), 400);
	}

	{
		AsyncFileHandle handle("sample2.bin");
		EXPECT_TRUE(handle.empty());
	}

	{
		AsyncFileQueue queue;

		AsyncFileHandle handle("sample.bin");

		Vector<s32> data(100);

		for (s32 i = 0; i < 100; ++i) {
			AsyncFileRequest request;
			request.handle = handle;
			request.offset = i * 4;
			request.size = 4;
			request.dest = data.data() + i;
			queue.add(request);
		}

		AsyncFileEvent event;
		queue.add(event);
		queue.submit();

		event.poll();
		event.wait();

		for (s32 i = 0; i < 100; ++i) {
			EXPECT_EQ(data[i], i);
		}

	}

	{
		AsyncFileQueue queue;

		AsyncFile file(queue, "sample.bin");

		queue.submit();

		file.poll();
		file.wait();

		EXPECT_EQ(file.size(),400);

		Span<s32> data((s32*)file.data(),100);

		for (s32 i = 0; i < 100; ++i) {
			EXPECT_EQ(data[i], i);
		}
	}

	File::Delete("sample.bin");

}