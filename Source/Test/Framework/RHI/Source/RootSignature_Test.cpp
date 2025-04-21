//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

class RootSignatureTest : public RHITestBase {};

// Emptyテスト
TEST_F(RootSignatureTest, Empty) {
	RootSignatureDesc desc;
	auto rootSignature = RootSignature::Create(desc);
	ASSERT_TRUE(rootSignature != nullptr);
}

// Slotテスト
TEST_F(RootSignatureTest, Resources) {
    RootSignatureDesc desc;
    desc.slots = {
        {
            Binding::Texture(),
            Binding::RWTexture(),
            Binding::Buffer(),
            Binding::RWBuffer(),
            Binding::StructuredBuffer(),
            Binding::RWStructuredBuffer(),
            Binding::ByteAddressBuffer(),
            Binding::RWByteAddressBuffer(),
            Binding::ConstantBuffer(),
        }
    };
    auto rootSignature = RootSignature::Create(desc);
    ASSERT_TRUE(rootSignature != nullptr);
}
TEST_F(RootSignatureTest, Sampler) {
    RootSignatureDesc desc;
    desc.slots = {
        {
            Binding::Sampler(),
        }
    };
    auto rootSignature = RootSignature::Create(desc);
    ASSERT_TRUE(rootSignature != nullptr);
}
TEST_F(RootSignatureTest, Mixed) {
    RootSignatureDesc desc;
    desc.slots = {
        {
            Binding::Texture(),
            Binding::RWTexture(),
            Binding::Buffer(),
            Binding::RWBuffer(),
            Binding::StructuredBuffer(),
            Binding::RWStructuredBuffer(),
            Binding::ByteAddressBuffer(),
            Binding::RWByteAddressBuffer(),
            Binding::ConstantBuffer(),
            Binding::Sampler(),
        }
    };
    auto rootSignature = RootSignature::Create(desc);
    ASSERT_TRUE(rootSignature == nullptr);
}
TEST_F(RootSignatureTest, MultiSlots) {
    RootSignatureDesc desc;
    desc.slots = {
        {
            Binding::Texture(),
            Binding::RWTexture(),
            Binding::Buffer(),
            Binding::RWBuffer(),
            Binding::StructuredBuffer(),
            Binding::RWStructuredBuffer(),
            Binding::ByteAddressBuffer(),
            Binding::RWByteAddressBuffer(),
            Binding::ConstantBuffer(),
        },
        {
            Binding::Texture(),
            Binding::RWTexture(),
            Binding::Buffer(),
            Binding::RWBuffer(),
            Binding::StructuredBuffer(),
            Binding::RWStructuredBuffer(),
            Binding::ByteAddressBuffer(),
            Binding::RWByteAddressBuffer(),
            Binding::ConstantBuffer(),
        },
        {
            Binding::Sampler(),
        }
    };
    auto rootSignature = RootSignature::Create(desc);
    ASSERT_TRUE(rootSignature != nullptr);
}

// StaticSamplerテスト
TEST_F(RootSignatureTest, StaticSampler) {
    RootSignatureDesc desc;
    desc.samplers = {
        StaticSamplerDesc(SamplerDesc(),0) ,
        StaticSamplerDesc(SamplerDesc(),1) ,
    };
    auto rootSignature = RootSignature::Create(desc);
    ASSERT_TRUE(rootSignature != nullptr);
}

// StaticSamplerテスト
TEST_F(RootSignatureTest, RootConstants) {
    RootSignatureDesc desc;
    desc.constants = {
        RootConstantsDesc(128,0)
    };
    auto rootSignature = RootSignature::Create(desc);
    ASSERT_TRUE(rootSignature != nullptr);
}

// Flagsテスト
TEST_F(RootSignatureTest, Flags) {
    RootSignatureDesc desc;
	desc.flags = RootSignatureFlag::EnableBindless;
    auto rootSignature = RootSignature::Create(desc);
    ASSERT_TRUE(rootSignature != nullptr);
}