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
	auto rootSignature = RootSignature::Create({});
	ASSERT_TRUE(rootSignature != nullptr);
}

// Slotテスト
TEST_F(RootSignatureTest, Resources) {

	Ref<DescriptorLayout> layout = DescriptorLayout::Create(
		{
			Binding::Texture(0),
			Binding::RWTexture(1),
			Binding::Buffer(2),
			Binding::RWBuffer(3),
			Binding::StructuredBuffer(4),
			Binding::RWStructuredBuffer(5),
			Binding::ByteAddressBuffer(6),
			Binding::RWByteAddressBuffer(7),
			Binding::ConstantBuffer(8),
		}
	);

	auto rootSignature = RootSignature::Create({ layout });
	ASSERT_TRUE(rootSignature != nullptr);

}
TEST_F(RootSignatureTest, Sampler) {


	Ref<DescriptorLayout> layout = DescriptorLayout::Create(
		{
			Binding::Sampler(0),
		}
	);

	auto rootSignature = RootSignature::Create({ layout });
	ASSERT_TRUE(rootSignature != nullptr);

}
TEST_F(RootSignatureTest, Mixed) {

	Ref<DescriptorLayout> layout = DescriptorLayout::Create(
		{
			Binding::Texture(0),
			Binding::RWTexture(1),
			Binding::Buffer(2),
			Binding::RWBuffer(3),
			Binding::StructuredBuffer(4),
			Binding::RWStructuredBuffer(5),
			Binding::ByteAddressBuffer(6),
			Binding::RWByteAddressBuffer(7),
			Binding::ConstantBuffer(8),
			Binding::Sampler(9),
		}
	);

	// NOTE Mixedも追加対応予定
	// auto rootSignature = RootSignature::Create({ layout });
	// ASSERT_TRUE(rootSignature == nullptr);

}
TEST_F(RootSignatureTest, MultiSlots) {

	Ref<DescriptorLayout> layout0 = DescriptorLayout::Create(
		{
			Binding::Texture(0),
			Binding::RWTexture(1),
			Binding::Buffer(2),
			Binding::RWBuffer(3),
			Binding::StructuredBuffer(4),
			Binding::RWStructuredBuffer(5),
			Binding::ByteAddressBuffer(6),
			Binding::RWByteAddressBuffer(7),
			Binding::ConstantBuffer(8),
		}
	);
	
	Ref<DescriptorLayout> layout1 = DescriptorLayout::Create(
		{
			Binding::Texture(10),
			Binding::RWTexture(11),
			Binding::Buffer(12),
			Binding::RWBuffer(13),
			Binding::StructuredBuffer(14),
			Binding::RWStructuredBuffer(15),
			Binding::ByteAddressBuffer(16),
			Binding::RWByteAddressBuffer(17),
			Binding::ConstantBuffer(18),
		}
	);
	
	Ref<DescriptorLayout> layout2 = DescriptorLayout::Create(
		{
			Binding::Sampler(20),
		}
	);

	auto rootSignature = RootSignature::Create({layout0,layout1,layout2});
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