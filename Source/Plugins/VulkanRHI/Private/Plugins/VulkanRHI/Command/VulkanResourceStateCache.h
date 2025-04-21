//***********************************************************
//! @file
//! @brief		リソースステート・キャッシュ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/VulkanRHI/Texture/VulkanTexture.h>
#include <Plugins/VulkanRHI/Buffer/VulkanBuffer.h>

namespace ob::rhi::vulkan {

    //! @brief  リソースステート・キャッシュ
    class VulkanResourceStateCache {
    public:

        //! @brief  コンストラクタ
        VulkanResourceStateCache() {

        }

        //! @brief  テクスチャの遷移追加
        void addTexture(vk::Image image, vk::ImageLayout before, vk::ImageLayout after, vk::ImageAspectFlagBits aspectMask) {

            auto& barrier = m_barriers.emplace_back();
            barrier = vk::ImageMemoryBarrier();
            barrier.srcAccessMask = {};
            barrier.dstAccessMask = {};
            barrier.oldLayout = before;
            barrier.newLayout = after;
            barrier.srcQueueFamilyIndex = 0;
            barrier.dstQueueFamilyIndex = 0;
            barrier.image = image;
            barrier.subresourceRange.aspectMask = aspectMask;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

        }

        //! @brief  コマンドを記録
        void recordCommand(vk::CommandBuffer commandBuffer) {

            if (m_barriers.empty()) {
                return;
            }

            commandBuffer.pipelineBarrier(
                vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eBottomOfPipe,
                {},
                {}, {}, m_barriers
            );

            m_barriers.clear();

            clear();

        }

        //! @brief  追加済みのバリアをコマンドリストに記録
        void clear() {
            m_barriers.clear();
        }

    private:

        Vector<vk::ImageMemoryBarrier> m_barriers;

    };

}