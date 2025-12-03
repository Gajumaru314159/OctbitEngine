//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

using namespace ob;
using namespace ob::rhi;

TYPED_TEST(RHITest, Sampler_Create) {

    for (auto filter : magic_enum::enum_values<TextureFilter>()) {

        for (auto mipFiler : magic_enum::enum_values<MipFilter>()) {

            for (auto address : magic_enum::enum_values<TextureAddress>()) {
                
                SamplerDesc desc;
                desc.filter = filter;
				desc.addressU = address;
				desc.mipFilter = mipFiler;
                desc.anisotropy = Anisotropy::None;

				auto sampler = Sampler::Create(desc);

				ASSERT_NE(sampler, nullptr);

            }

        }

    }

}