//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/CommandList.h>

namespace ob::rhi {

    //! @brief  コマンドキュー
    class VulkanCommandQueue {
    public:

        //! @brief  コンストラクタ
        VulkanCommandQueue(VulkanRHI& rhi);

        void entryCommandList(const Ref<CommandList>& commandList);
        void entryCommandListTop(const Ref<CommandList>& commandList);

        void execute();
        void wait();

		vk::Queue getQueue() const {
			return *m_queue;
		}

    private:
        VulkanRHI&                  m_rhi;
        vk::raii::Queue             m_queue = nullptr;

        Vector<Ref<CommandList>>    m_entriedCommandListVector;

        Vector<vk::CommandBuffer>   m_commandBuffers;
    };

}