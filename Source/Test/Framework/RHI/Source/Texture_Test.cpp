//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

class TextureTest : public RHITestBase {};

TEST_F(TextureTest, Create) {

	// タイプ
    for (auto type : magic_enum::enum_values<TextureType>()) {

		TextureDesc desc;
		desc.type = type;
		if (type == TextureType::Texture1D) desc.size = Size{ 100 };
		if (type == TextureType::Texture2D) desc.size = Size{ 100,100 };
		if (type == TextureType::Texture3D) desc.size = Size{ 100,100,100 };
		if (type == TextureType::Cube) desc.size = Size{ 100,100 };

		auto texture = Texture::Create(desc);

        ASSERT_NE(texture, nullptr);

    }

	// フォーマット
	for (auto format : magic_enum::enum_values<TextureFormat>()) {

		TextureDesc desc;
		desc.format = format;
		desc.size = { 100, 100 };

		auto texture = Texture::Create(desc);

		if (!Texture::Supports(format)) {
			ASSERT_EQ(texture, nullptr);
			continue;
		}

		//if (!texture)CallBreakPoint();
		ASSERT_NE(texture, nullptr);

	}

	// 不正サイズ
	{
		TextureDesc desc;
		desc.size = { -100, -100 };

		auto texture = Texture::Create(desc);

		ASSERT_EQ(texture, nullptr);
	}

	// 配列
	{
		TextureDesc desc;
		desc.size = { -100, -100 };
		desc.arrayNum = 4;

		auto texture = Texture::Create(desc);

		ASSERT_EQ(texture, nullptr);
	}

	for (s32 i = 0; i < 10; ++i) {

		TextureDesc desc;
		desc.size = { 1024, 600};
		desc.arrayNum = i;

		auto texture = Texture::Create(desc);

		ASSERT_NE(texture, nullptr);

	}

}
