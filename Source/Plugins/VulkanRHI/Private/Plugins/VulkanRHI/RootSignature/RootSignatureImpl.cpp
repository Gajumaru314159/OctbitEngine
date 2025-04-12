//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>
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
	RootSignatureImpl::RootSignatureImpl(VulkanRHI& rhi, const BindingLayoutDesc& desc)
		: m_desc(desc)
	{
		m_desc.normalize();

		auto& device = rhi.getDevice();

		FixedVector<vk::DescriptorSetLayout,32> layouts;		

		for (auto& slot : desc.slots) {

			FixedVector<vk::DescriptorSetLayoutBinding, 32> bindings;

			for (auto& item : slot.items) {

				auto& binding = bindings.emplace_back();
				binding.binding = item.index;
				binding.descriptorType = Convert(item.type);
				binding.descriptorCount = 1;
				binding.stageFlags = vk::FlagTraits<vk::ShaderStageFlagBits>::allFlags;
				binding.pImmutableSamplers = nullptr;

			}

			vk::DescriptorSetLayoutCreateInfo info;
			info.setBindings(bindings);

			m_layouts.push_back(device.createDescriptorSetLayout(info, rhi.getAllocationCallbacks()));
			layouts.push_back(m_layouts.back());

		}

		vk::PushConstantRange pushConstantRange;
		pushConstantRange.stageFlags = vk::FlagTraits<vk::ShaderStageFlagBits>::allFlags;
		pushConstantRange.offset = 0;
		pushConstantRange.size = desc.constants.size;

		vk::PipelineLayoutCreateInfo createInfo;
		createInfo.setLayoutCount = (u32)layouts.size();
		createInfo.pSetLayouts = layouts.data();
		createInfo.pushConstantRangeCount = 1;
		if (0 < m_desc.constants.size) {
			createInfo.pPushConstantRanges = &pushConstantRange;
		}

		m_pipelineLayout = device.createPipelineLayout(createInfo, rhi.getAllocationCallbacks());
	}


	//! @brief  デストラクタ
	RootSignatureImpl::~RootSignatureImpl() {

	}
	
	//! @brief      名前を取得
	const String& RootSignatureImpl::getName()const {
		return m_desc.name;
	}


	//! @brief  定義を取得
	const BindingLayoutDesc& RootSignatureImpl::getDesc()const noexcept {
		return m_desc;
	}

}