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


	// bufferType
	for (auto type : magic_enum::enum_values<BufferType>()) {

		// usage
		for (auto usage : magic_enum::enum_values<ResourceUsage>()) {

			// bufferSize
			size_t sizes[] = { 0 ,100,256,512 };
			for (auto size : sizes) {

				// bufferStride
				for (u32 stride = 0; stride < 256; stride += 64) {

					// bufferFlags
					for (auto flags : magic_enum::enum_values<BufferFlag>()) {

						// bindFlags
						for (auto bind : magic_enum::enum_values<BindFlag>()) {

							size_t checkSize = size;

							BufferDesc desc;
							desc.bufferType = type;
							desc.usage = usage;
							desc.bufferSize = size;
							desc.bufferStride = stride;
							desc.bufferFlags = flags;
							desc.bindFlags = bind;
							Ref<Buffer> buffer = Buffer::Create(desc);

							// サイズチェック
							if (size == 0) {
								checkSize = 256;
							}

							// 256バイト制限
							if (size %256 != 0 && type == BufferType::ConstantBuffer) {
								checkSize = align_up(checkSize,256);
							}

							ASSERT_NE(buffer, nullptr);
							EXPECT_EQ(buffer->getDesc().bufferType, type);
							EXPECT_EQ(buffer->getDesc().usage, usage);
							EXPECT_EQ(buffer->getDesc().bufferSize, checkSize);
							EXPECT_EQ(buffer->getDesc().bufferStride, stride);
							EXPECT_EQ(buffer->getDesc().bufferFlags, BufferFlags(flags));
							EXPECT_EQ(buffer->getDesc().bindFlags, BindFlags(bind));

						}
					}
				}
			}
		}

	}

}

TEST_F(BufferTest, CreateUtility) {

	// Constant
	{
		BufferDesc desc = BufferDesc::Constant(100, BindFlag::PixelShaderResource);

		Ref<Buffer> buffer = Buffer::Create(desc);
		ASSERT_NE(buffer, nullptr);
	}

	// Vertex
	{
		BufferDesc desc = BufferDesc::Vertex<Vec4>(100);
		Ref<Buffer> buffer = Buffer::Create(desc);
		ASSERT_NE(buffer, nullptr);
		ASSERT_EQ(buffer->getDesc().bufferSize, sizeof(Vec4) * 100);
	}

	// Index
	{
		BufferDesc desc = BufferDesc::Index<u16>(100);
		Ref<Buffer> buffer = Buffer::Create(desc);
		ASSERT_NE(buffer, nullptr);
		ASSERT_EQ(buffer->getDesc().bufferSize,sizeof(u16)*100);
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