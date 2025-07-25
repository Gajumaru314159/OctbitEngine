//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorLayout.h>
#include <Plugins/VulkanRHI/VulkanDevice.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi {

	//! @brief              コンストラクタ
	VulkanDescriptorLayout::VulkanDescriptorLayout(VulkanDevice& device, const DescriptorLayoutDesc& desc)
		: m_desc(desc)
	{
		auto& vkdevice = device.getDevice();

		FixedVector<vk::DescriptorSetLayoutBinding, 32> bindings;

		// NOTE bindingが連続していればまとめられるが処理が複雑になるので1つずつ登録する
		//      パフォーマンス上の問題があればまとめる

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

		m_layout = vkdevice.createDescriptorSetLayout(info, device.getAllocationCallbacks());

		manage();
	}

}