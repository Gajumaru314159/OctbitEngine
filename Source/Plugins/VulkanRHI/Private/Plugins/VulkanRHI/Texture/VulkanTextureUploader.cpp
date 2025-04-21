//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Texture/VulkanTextureUploader.h>
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

		OB_ASSERT(subresources.size()==1,"Mipmapは未実装です");

		auto& device = m_rhi.getDevice();


		size_t bufferSize = info.extent.width;

		vk::BufferCreateInfo bufferCreateInfo({}, bufferSize, vk::BufferUsageFlagBits::eTransferSrc);
		vk::raii::Buffer buffer = device.createBuffer(bufferCreateInfo,m_rhi.getAllocationCallbacks());

		auto allocInfo = m_rhi.getAllocationInfo(buffer.getMemoryRequirements(), vk::MemoryPropertyFlags{} | vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		vk::raii::DeviceMemory memory = device.allocateMemory(allocInfo, m_rhi.getAllocationCallbacks());

		buffer.bindMemory(memory, 0);

		void* data = memory.mapMemory(0, bufferSize, vk::MemoryMapFlags{});

		for (auto& subresource : subresources) {
			memcpy_s(data,bufferSize,subresource.data.data(),subresource.data.size());
		}

		memory.unmapMemory();


		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		auto& request = frame.requests.emplace_back();
		request.source = std::move(buffer);
		request.memory = std::move(memory);
		request.dest = dest;
		// request.destLayout;

	}

	void TextureUploader::update(vk::CommandBuffer commandBuffer) {

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