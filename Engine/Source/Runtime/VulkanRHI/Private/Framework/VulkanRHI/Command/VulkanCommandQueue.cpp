//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/VulkanRHI/Command/VulkanCommandQueue.h>
#include <Framework/VulkanRHI/Command/VulkanCommandList.h>
#include <Framework/VulkanRHI/VulkanDevice.h>

namespace ob::rhi {

	//! @brief  コンストラクタ
	VulkanCommandQueue::VulkanCommandQueue(VulkanDevice& device) 
		: m_device(device)
	{
		auto& vkdevice = device.getDevice();
		m_queue = vkdevice.getQueue(device.getQueryFamilyIndex(), 0);
		m_device.setName(m_queue, "VulkanCommandQueue");
	}

	//! @brief コマンドリストを登録 
	void VulkanCommandQueue::entryCommandList(const Ref<CommandList>& commandList) {
		m_enteredCommandListVector.push_back(commandList);
	}

	//! @brief コマンドリストを先頭に登録
	void VulkanCommandQueue::entryCommandListTop(const Ref<CommandList>& commandList) {
		m_enteredCommandListVector.insert(m_enteredCommandListVector.begin(), commandList);
	}

	//! @brief 登録したコマンドリストを実行
	void VulkanCommandQueue::execute() {

		m_commandBuffers.clear();
		for (auto& commandList : m_enteredCommandListVector) {
			if (auto native = commandList.cast<VulkanCommandList>()) {
				m_commandBuffers.push_back(native->getNative());
			}
		}
		m_enteredCommandListVector.clear();

		vk::SubmitInfo submitInfo;
		submitInfo.setCommandBuffers(m_commandBuffers);

		m_queue.submit(submitInfo);

	}

	//! @brief 待機
	void VulkanCommandQueue::wait() const {
		m_queue.waitIdle();
	}

}