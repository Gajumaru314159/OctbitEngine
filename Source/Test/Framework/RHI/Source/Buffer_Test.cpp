//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

using namespace ob;
using namespace ob::rhi;

class BufferTest : public RHITestBase {};

TEST_F(BufferTest, Create) {

	for (auto type : magic_enum::enum_values<BufferState>()) {

		size_t sizes[] = { 0 ,100,256,512 };
		for (auto size : sizes) {

			for (u32 stride = 0; stride < 256; stride += 64) {

				for (auto flags : magic_enum::enum_values<BufferFlag>()) {

					size_t checkSize = size;

					BufferDesc desc;
					desc.state = type;
					desc.size = size;
					desc.stride = stride;
					desc.flags = flags;
					Ref<Buffer> buffer = Buffer::Create(desc);

					// サイズチェック
					if (!desc.isValid()) {
						ASSERT_FALSE(buffer);
						continue;
					}

					ASSERT_NE(buffer, nullptr);
					EXPECT_EQ(buffer->getDesc().state, type);
					EXPECT_EQ(buffer->getDesc().size, checkSize);
					EXPECT_EQ(buffer->getDesc().stride, stride);
					EXPECT_EQ(buffer->getDesc().flags, BufferFlags(flags));

				}
			}
		}

	}

}

TEST_F(BufferTest, CreateUtility) {

	// Constant
	{
		BufferDesc desc = BufferDesc::Constant(100);

		Ref<Buffer> buffer = Buffer::Create(desc);
		ASSERT_TRUE(buffer);
	}
	// Constant
	{
		BufferDesc desc = BufferDesc::Constant(100);
		desc.size--;

		Ref<Buffer> buffer = Buffer::Create(desc);
		ASSERT_FALSE(buffer);
	}

	// Vertex
	{
		BufferDesc desc = BufferDesc::Vertex<Vec4>(100);
		Ref<Buffer> buffer = Buffer::Create(desc);
		ASSERT_NE(buffer, nullptr);
		ASSERT_EQ(buffer->getDesc().size, sizeof(Vec4) * 100);
	}

	// Index
	{
		BufferDesc desc = BufferDesc::Index<u16>(100);
		Ref<Buffer> buffer = Buffer::Create(desc);
		ASSERT_NE(buffer, nullptr);
		ASSERT_EQ(buffer->getDesc().size,sizeof(u16)*100);
	}

}

TEST_F(BufferTest, Update) {

	// バッファの作成
	Ref<Buffer> buffer = Buffer::Create(BufferDesc::Vertex<f32>(6));
	ASSERT_NE(buffer, nullptr);

	// バッファの更新
	Array<f32,6> data = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

	// 直接渡し
	buffer->updateDirect(data.size() * sizeof(float), data.data(), 0);

	// 関数渡し
	buffer->updateDirect([&](void* ptr) {
		memcpy(ptr, data.data(), data.size() * sizeof(float));
	});

	// Template
	buffer->updateDirect(data, 0);

}