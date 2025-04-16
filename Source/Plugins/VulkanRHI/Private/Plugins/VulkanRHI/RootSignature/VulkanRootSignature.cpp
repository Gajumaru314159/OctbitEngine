//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/RootSignature/VulkanRootSignature.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>

namespace ob::rhi::vulkan {
	static  vk::DescriptorType Convert(BindingType value) {
		switch (value)
		{
		case BindingType::Texture:
			return vk::DescriptorType::eSampledImage;
		case BindingType::RWTexture:
			return vk::DescriptorType::eStorageImage;
		case BindingType::Buffer:
		case BindingType::StructuredBuffer:
		case BindingType::ByteAddressBuffer:
			return vk::DescriptorType::eUniformBuffer;
		case BindingType::RWBuffer:
		case BindingType::RWStructuredBuffer:
		case BindingType::RWByteAddressBuffer:
			return vk::DescriptorType::eStorageBuffer;
		case BindingType::ConstantBuffer:
			return vk::DescriptorType::eUniformBuffer;
		case BindingType::Sampler:
			return vk::DescriptorType::eSampler;
		}
		OB_ABORT("不正なBindingTypeです。");
		return {};
	}


	//! @brief  コンストラクタ
	VulkanRootSignature::VulkanRootSignature(VulkanRHI& rhi, const RootSignatureDesc& desc)
		: m_desc(desc)
	{
		m_desc.normalize();

		auto& device = rhi.getDevice();

		m_layouts.reserve(m_desc.slots.size());

		FixedVector<vk::DescriptorSetLayout,32> layouts;		

		for (auto& slot : m_desc.slots) {

			FixedVector<vk::DescriptorSetLayoutBinding, 32> bindings;

			s32 resourceCount = 0;
			s32 samplerCount = 0;

			for (auto& item : slot.items) {

				auto& binding = bindings.emplace_back();
				binding.binding = item.index;
				binding.descriptorType = Convert(item.type);
				binding.descriptorCount = 1;
				binding.stageFlags = vk::FlagTraits<vk::ShaderStageFlagBits>::allFlags;
				binding.pImmutableSamplers = nullptr;

				if (item.type != BindingType::Sampler) resourceCount++;
				if (item.type == BindingType::Sampler) samplerCount++;

			}

			if (0 < resourceCount && 0 < samplerCount) {
				throw Exception("BindingSlot内にSamplerとそれ以外を混在させることはできません");
			}

			vk::DescriptorSetLayoutCreateInfo info;
			info.setBindings(bindings);

			m_layouts.push_back(device.createDescriptorSetLayout(info, rhi.getAllocationCallbacks()));
			layouts.push_back(*m_layouts.back());

		}

		vk::PushConstantRange pushConstantRange;
		pushConstantRange.stageFlags = vk::FlagTraits<vk::ShaderStageFlagBits>::allFlags;
		pushConstantRange.offset = 0;
		pushConstantRange.size = m_desc.constants.size;

		vk::PipelineLayoutCreateInfo createInfo;
		createInfo.setLayoutCount = (u32)layouts.size();
		createInfo.pSetLayouts = layouts.empty() ? nullptr : layouts.data();
		if (0 < m_desc.constants.size) {
			createInfo.pushConstantRangeCount = 1;
			createInfo.pPushConstantRanges = &pushConstantRange;
		}

		m_pipelineLayout = device.createPipelineLayout(createInfo, rhi.getAllocationCallbacks());
	}


	//! @brief  デストラクタ
	VulkanRootSignature::~VulkanRootSignature() {

	}
	
	//! @brief      名前を取得
	const String& VulkanRootSignature::getName()const {
		return m_desc.name;
	}


	//! @brief  定義を取得
	const RootSignatureDesc& VulkanRootSignature::getDesc()const noexcept {
		return m_desc;
	}

}