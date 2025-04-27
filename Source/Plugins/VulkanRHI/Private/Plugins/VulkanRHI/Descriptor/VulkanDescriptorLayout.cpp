//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorLayout.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi
{

	//! @brief              コンストラクタ
	VulkanDescriptorLayout::VulkanDescriptorLayout(VulkanRHI& rhi, const DescriptorLayoutDesc& desc)
		: m_desc(desc)
	{

		auto& device = rhi.getDevice();

		FixedVector<vk::DescriptorSetLayoutBinding, 32> bindings;

		s32 resourceCount = 0;
		s32 samplerCount = 0;

		for (auto [index, item] : Indexed(desc.items)) {
			auto& binding = bindings.emplace_back();
			binding.binding = item.index;
			binding.descriptorType = TypeConverter::Convert(item.type);
			binding.descriptorCount = 1;
			binding.stageFlags = vk::FlagTraits<vk::ShaderStageFlagBits>::allFlags;
			binding.pImmutableSamplers = nullptr;
		}

		vk::DescriptorSetLayoutCreateInfo info;
		info.setBindings(bindings);


		//std::vector<vk::DescriptorBindingFlags> bindFlag(vulkanLayoutBindings.size(), vk::DescriptorBindingFlagBits::ePartiallyBound);
		//
		//auto extendedInfo = vk::DescriptorSetLayoutBindingFlagsCreateInfo()
		//	.setBindingCount(uint32_t(vulkanLayoutBindings.size()))
		//	.setPBindingFlags(bindFlag.data());
		//
		//if (rhi.getConfig().enableBindless) {
		//
		//}


		m_layout = device.createDescriptorSetLayout(info, rhi.getAllocationCallbacks());

		manage();
	}

}