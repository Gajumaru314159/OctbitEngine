//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>

namespace ob::rhi::vulkan {

	static  vk::DescriptorType Convert(DescriptorRangeType type) {
		switch (type)
		{
		case ob::rhi::DescriptorRangeType::Texture_SRV: 
			return vk::DescriptorType::eSampledImage;
		case ob::rhi::DescriptorRangeType::Texture_UAV:
			return vk::DescriptorType::eStorageImage;
		case ob::rhi::DescriptorRangeType::TypedBuffer_SRV:
			return vk::DescriptorType::eUniformTexelBuffer;
		case ob::rhi::DescriptorRangeType::TypedBuffer_UAV:
			return vk::DescriptorType::eStorageTexelBuffer;
		case ob::rhi::DescriptorRangeType::StructuredBuffer_SRV:
			return vk::DescriptorType::eStorageBuffer;
		case ob::rhi::DescriptorRangeType::StructuredBuffer_UAV:
			return vk::DescriptorType::eStorageBuffer;
		case ob::rhi::DescriptorRangeType::RawBuffer_SRV:
			return vk::DescriptorType::eStorageBuffer;
		case ob::rhi::DescriptorRangeType::RawBuffer_UAV:
			return vk::DescriptorType::eStorageBuffer;
		case ob::rhi::DescriptorRangeType::ConstantBuffer:
			return vk::DescriptorType::eUniformBuffer;
		case ob::rhi::DescriptorRangeType::Sampler:
			return vk::DescriptorType::eSampler;
		}
		throw Exception("不正なDescriptorRangeTypeです");
	}


	//! @brief  コンストラクタ
	RootSignatureImpl::RootSignatureImpl(VulkanRHI& rhi, const RootSignatureDesc& desc)
		: m_desc(desc)
	{
		auto& device = rhi.getDevice();

		FixedVector<vk::DescriptorSetLayout,32> layouts;

		for (auto& parameter : desc.parameters) {

			vk::DescriptorSetLayoutBinding binding;
			binding.binding = parameter.range.baseRegister;
			binding.descriptorType = Convert(parameter.range.type);
			binding.descriptorCount = parameter.range.num;
			binding.stageFlags = vk::FlagTraits<vk::ShaderStageFlagBits>::allFlags;
			binding.pImmutableSamplers = nullptr;

			vk::DescriptorSetLayoutCreateInfo info;
			info.setBindings(binding);

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
		createInfo.pPushConstantRanges = &pushConstantRange;

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
	const RootSignatureDesc& RootSignatureImpl::getDesc()const noexcept {
		return m_desc;
	}

}