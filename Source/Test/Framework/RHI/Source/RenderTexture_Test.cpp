//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

using namespace ob;
using namespace ob::rhi;

TYPED_TEST(RHITest, RenderTexture_Create) {
    return;
    for (auto format : magic_enum::enum_values<TextureFormat>()) {

        Size sizes[] = {
            { 100, 100},
            { 200, 400},
        };

        for (auto size : sizes) {
            //if (format == TextureFormat::D24S8)CallBreakPoint();

            RenderTextureDesc desc;
            desc.name = Format("{} {}x{}",magic_enum::enum_name(format),size.width,size.height);
			desc.format = format;
			desc.size = size;
			auto renderTexture = RenderTexture::Create(desc);

            if (!RenderTexture::Supports(format)) {
                ASSERT_EQ(renderTexture, nullptr);
                continue;
            }

            //if (!renderTexture)CallBreakPoint();
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
