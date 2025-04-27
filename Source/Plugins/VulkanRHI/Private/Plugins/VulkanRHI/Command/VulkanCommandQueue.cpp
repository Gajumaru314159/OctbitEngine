//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Command/VulkanCommandQueue.h>
#include <Plugins/VulkanRHI/Command/VulkanCommandList.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>

namespace ob::rhi {


	//! @brief  コンストラクタ
	VulkanCommandQueue::VulkanCommandQueue(VulkanRHI& rhi) 
		: m_rhi(rhi)
	{
		auto& device = rhi.getDevice();
		m_queue = device.getQueue(rhi.getQueryFamilyIndex(), 0);
		m_rhi.setName(m_queue, "VulkanCommandQueue");
	}

	//! @brief コマンドリストを登録 
	void VulkanCommandQueue::entryCommandList(const Ref<CommandList>& commandList) {
		m_entriedCommandListVector.push_back(commandList);
	}

	//! @brief コマンドリストを先頭に登録
	void VulkanCommandQueue::entryCommandListTop(const Ref<CommandList>& commandList) {
		m_entriedCommandListVector.insert(m_entriedCommandListVector.begin(), commandList);
	}

	//! @brief 登録したコマンドリストを実行
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

	//! @brief 待機
	void VulkanCommandQueue::wait() {
		m_queue.waitIdle();
	}

}