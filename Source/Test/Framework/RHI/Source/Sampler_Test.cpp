//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

class SamplerTest : public RHITestBase {};

TEST_F(SamplerTest, Create) {

    for (auto filter : magic_enum::enum_values<TextureFillter>()) {

        for (auto mipFiler : magic_enum::enum_values<MipFillter>()) {

            for (auto address : magic_enum::enum_values<TextureAddress>()) {
                
                SamplerDesc desc;
                desc.filter = filter;
				desc.addressU = address;
				desc.mipFilter = mipFiler;

				auto sampler = Sampler::Create(desc);

				ASSERT_NE(sampler, nullptr);

            }

        }

    }

}