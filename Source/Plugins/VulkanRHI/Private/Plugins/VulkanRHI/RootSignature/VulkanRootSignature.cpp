//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/RootSignature/VulkanRootSignature.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorLayout.h>

namespace ob::rhi {

	//! @brief  コンストラクタ
	VulkanRootSignature::VulkanRootSignature(VulkanRHI& rhi, const RootSignatureDesc& desc)
		: m_desc(desc)
	{
		auto& device = rhi.getDevice();

		// NOTE ここのlayouts[i]がlayout(set=i)と対応する
		FixedVector<vk::DescriptorSetLayout,32> layouts;		

		// Bindfull用のDescriptorSetLayoutを追加
		for (auto& layout : m_desc.layouts) {
			OB_ASSERT(layout.cast<VulkanDescriptorLayout>(),"DescriptorLayoutが取得できません");
			layouts.push_back(layout.cast<VulkanDescriptorLayout>()->getNative());
		}

		// Bindless用のDescriptorSetLayoutはシェーダーコンパイル時点でsetを確定させるためにset=0で固定する
		if (rhi.getConfig().enableBindless) {
			layouts.push_back(rhi.getBindlessDescriptorSetLayout());
		}

		vk::PushConstantRange pushConstantRange;
		pushConstantRange.stageFlags = vk::FlagTraits<vk::ShaderStageFlagBits>::allFlags;
		pushConstantRange.offset = 0;
		pushConstantRange.size = m_desc.constants.size;

		vk::PipelineLayoutCreateInfo createInfo;
		createInfo.setLayoutCount = layouts.size();
		createInfo.pSetLayouts = layouts.empty() ? nullptr : layouts.data();
		if (0 < m_desc.constants.size) {
			createInfo.pushConstantRangeCount = 1;
			createInfo.pPushConstantRanges = &pushConstantRange;
		}

		m_pipelineLayout = device.createPipelineLayout(createInfo, rhi.getAllocationCallbacks());

		rhi.setName(m_pipelineLayout, m_desc.name);

		manage();
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