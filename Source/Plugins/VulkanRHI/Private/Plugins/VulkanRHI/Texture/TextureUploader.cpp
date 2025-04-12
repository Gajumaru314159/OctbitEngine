//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Texture/TextureUploader.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>

namespace ob::rhi::vulkan
{

	//! @brief  コンストラクタ
	TextureUploader::TextureUploader(VulkanRHI& rhi)
		: m_rhi(rhi)
	{
		m_frames.resize(4);
	}

	//! @brief  アップロード要素を追加
	void TextureUploader::add(const vk::raii::Image& dest, vk::ImageCreateInfo info, Span<Subresource> subresources) {

		if (dest == nullptr) {
			LOG_ERROR("[TextureUploader] destがnullです。");
			return;
		}
		auto& device = m_rhi.getDevice();


		auto& device = m_rhi.getDevice();
		auto allocationCallbacks = m_rhi.getAllocationCallbacks();

		// 転送用バッファを生成
		vk::BufferCreateInfo info;
		info.size = m_blockSize;
		info.usage = vk::BufferUsageFlagBits::eTransferSrc;
		info.sharingMode = vk::SharingMode::eExclusive;

		block.buffer = device.createBuffer(info, allocationCallbacks);

		// メモリ確保
		VkMemoryAllocateInfo allocInfo = m_rhi.getAllocationInfo(block.buffer.getMemoryRequirements(), vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		block.memory = device.allocateMemory(allocInfo, allocationCallbacks);

		// バインド
		block.buffer.bindMemory(block.memory, 0);

		Utility::SetName(device, block.buffer, "BufferUploader");
		Utility::SetName(device, block.memory, "BufferUploader");


		auto sourceResource = createUploadResource(info);
		if (sourceResource == nullptr) {
			LOG_ERROR("[TextureUploader] コピーソースリソースの生成に失敗しました");
			return;
		}

	}

    //! @brief アップロード用の一時リソースを作成 
	vk::raii::DeviceMemory TextureUploader::createUploadResource(const vk::BufferCreateInfo& info) {

		auto& device = m_rhi.getDevice();


		return nullptr;
	}


	void TextureUploader::update(vk::raii::CommandBuffer& commandBuffer) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		for (auto& request : frame.requests) {
			auto& barrier = m_barriers.emplace_back();
			barrier = vk::ImageMemoryBarrier();
			barrier.srcAccessMask = {};
			barrier.dstAccessMask = {};
			barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
			barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			barrier.srcQueueFamilyIndex = 0;
			barrier.dstQueueFamilyIndex = 0;
			barrier.image = request.dest;
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = request.mipLevels;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = request.layerCount;
		}


		for (auto& request : frame.requests) {

			VkDeviceSize                bufferOffset;
			uint32_t                    bufferRowLength;
			uint32_t                    bufferImageHeight;
			VkImageSubresourceLayers    imageSubresource;
			VkOffset3D                  imageOffset;
			VkExtent3D                  imageExtent;

			vk::BufferImageCopy region;
			region.bufferOffset = 0;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;
			region.imageSubresource = vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor,request.mipLevels,0,request.layerCount);
			region.imageOffset = 0;
			region.imageExtent = vk::Extent3D();

			commandBuffer.copyBufferToImage(request.source, request.dest, request.destLayout, region);

		}


	}

}