//***********************************************************
//! @file
//! @brief		コマンドキュー
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Command/VulkanCommandQueue.h>
#include <Plugins/VulkanRHI/Command/VulkanCommandList.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>

namespace ob::rhi::vulkan {


	//! @brief  コンストラクタ
	VulkanCommandQueue::VulkanCommandQueue(VulkanRHI& rhi) 
		: m_rhi(rhi)
	{
		auto& device = rhi.getDevice();
		m_queue = device.getQueue(rhi.getQueryFamilyIndex(), 0);
	}

	void VulkanCommandQueue::entryCommandList(const Ref<CommandList>& commandList) {
		m_entriedCommandListVector.push_back(commandList);
	}
	void VulkanCommandQueue::entryCommandListTop(const Ref<CommandList>& commandList) {
		m_entriedCommandListVector.insert(m_entriedCommandListVector.begin(), commandList);
	}

	void VulkanCommandQueue::execute() {

		m_commandBuffers.clear();
		for (auto& commandList : m_entriedCommandListVector) {
			if (auto native = commandList.cast<VulkanCommandList>()) {
				m_commandBuffers.push_back(native->getNative());
			}
		}
		m_entriedCommandListVector.clear();

		vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
		vk::SubmitInfo submitInfo;
		submitInfo.setCommandBuffers(m_commandBuffers);

		m_queue.submit(submitInfo);

	}

	void VulkanCommandQueue::wait() {
		m_queue.waitIdle();
	}

}