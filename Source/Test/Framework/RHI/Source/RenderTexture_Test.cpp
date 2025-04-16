//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>

class RenderTextureTest : public RHITestBase {};

TEST_F(RenderTextureTest, Create) {
    return;
    for (auto format : magic_enum::enum_values<TextureFormat>()) {

        Size sizes[] = {
            { 100, 100},
            { 200, 400},
        };

        for (auto size : sizes) {
            //if (format == TextureFormat::D24S8)CallBreakPoint();
            // format = TextureFormat::D24S8;

            // if(!RenderTexture::Supports(format))continue;

            RenderTextureDesc desc;
            desc.name = Format("{} {}x{}",magic_enum::enum_name(format),size.width,size.height);
			desc.format = format;
			desc.size = size;
			auto renderTexture = RenderTexture::Create(desc);

            if (format == TextureFormat::Unknown) {
                ASSERT_EQ(renderTexture, nullptr);
                continue;
            }
            if (format == TextureFormat::RGB32) {
                ASSERT_EQ(renderTexture, nullptr);
                continue;
            }
            if (format == TextureFormat::RGB8) {
                ASSERT_EQ(renderTexture, nullptr);
                continue;
            }
            if (format == TextureFormat::D24S8) {
                // Tier次第で生成に失敗するので除外
                //ASSERT_EQ(renderTexture, nullptr);
                continue;
            }
            if (format == TextureFormat::D16) {
                // Tier次第で生成に失敗するので除外
                //ASSERT_EQ(renderTexture, nullptr);
                continue;
            }
            if (TextureFormatUtility::IsBC(format)) {
                ASSERT_EQ(renderTexture, nullptr);
                continue;
            }

            if (!renderTexture)CallBreakPoint();
			ASSERT_NE(renderTexture, nullptr);

            ASSERT_EQ(renderTexture->descOfRenderTexture().format, format);
            ASSERT_EQ(renderTexture->descOfRenderTexture().size, size);


        }

    }


    // 不正サイズ
    {
        RenderTextureDesc desc;
        desc.size = { -100, -100 };

        auto texture = RenderTexture::Create(desc);

        ASSERT_EQ(texture, nullptr);
    }

}
