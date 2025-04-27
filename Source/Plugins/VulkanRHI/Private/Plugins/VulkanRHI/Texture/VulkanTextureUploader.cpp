//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Texture/VulkanTextureUploader.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Command/VulkanCommandList.h>

namespace ob::rhi
{

	//! @brief  コンストラクタ
	TextureUploader::TextureUploader(VulkanRHI& rhi)
		: m_rhi(rhi)
	{
		m_frames.resize(4);
	}

	//! @brief  アップロード要素を追加
	void TextureUploader::add(const vk::raii::Image& dest, vk::ImageCreateInfo info, TextureFormat format, Span<Subresource> subresources) {

		if (dest == nullptr) {
			LOG_ERROR("[TextureUploader] destがnullです。");
			return;
		}

		OB_ASSERT(subresources.size()==1,"Mipmapは未実装です");

		auto& device = m_rhi.getDevice();


		size_t bufferSize = 0;
		for (auto& subresource : subresources) {
			bufferSize += subresource.data.size();
		}

		// アップロード用のバッファを生成
		vk::BufferCreateInfo bufferCreateInfo({}, bufferSize, vk::BufferUsageFlagBits::eTransferSrc);
		vk::raii::Buffer buffer = device.createBuffer(bufferCreateInfo,m_rhi.getAllocationCallbacks());

		auto allocInfo = m_rhi.getAllocationInfo(buffer.getMemoryRequirements(), vk::MemoryPropertyFlags{} | vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
		vk::raii::DeviceMemory memory = device.allocateMemory(allocInfo, m_rhi.getAllocationCallbacks());
		buffer.bindMemory(memory, 0);

		// バッファにデータをコピー
		void* data = memory.mapMemory(0, bufferSize, vk::MemoryMapFlags{});

		for (auto& subresource : subresources) {
			memcpy_s(data,bufferSize,subresource.data.data(),subresource.data.size());
		}

		memory.unmapMemory();


		// リクエストを追加
		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		auto& request = frame.requests.emplace_back();
		request.source = std::move(buffer);
		request.memory = std::move(memory);
		request.dest = dest;
		request.mipLevels = subresources.size();
		request.layerCount = 1;
		request.format = format;
		request.extent = info.extent;

		m_rhi.setName(request.source, "VulkanTextureUploader");
		m_rhi.setName(request.memory, "VulkanTextureUploader");

	}

	//! @brief フレームごとのバッファ更新を行う
	void TextureUploader::update(Ref<CommandList>& commandList) {

		auto commandListImpl = commandList.cast<VulkanCommandList>();
		if (commandListImpl == nullptr) {
			return;
		}

		vk::CommandBuffer commandBuffer = commandListImpl->getNative();

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		commandList->pushMarker("TextureUploader");

		// バリア
		m_barriers.clear();
		for (auto& request : frame.requests) {

			vk::ImageAspectFlags flags{};
			if (TextureFormatUtility::HasColor(request.format)) flags |= vk::ImageAspectFlagBits::eColor;
			if (TextureFormatUtility::HasDepth(request.format)) flags |= vk::ImageAspectFlagBits::eDepth;
			if (TextureFormatUtility::HasStencil(request.format)) flags |= vk::ImageAspectFlagBits::eStencil;

			// 転送元
			vk::ImageMemoryBarrier barrier;
			barrier.srcAccessMask = vk::AccessFlagBits::eNone;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.oldLayout = vk::ImageLayout::eUndefined;
			barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = request.dest;
			barrier.subresourceRange.aspectMask = flags;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = request.mipLevels;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = request.layerCount;
			m_barriers.push_back(barrier);

			// 転送先は一次バッファなのでバリアは不用

		}

		// バリア追加
		if (!m_barriers.empty()) {
			commandBuffer.pipelineBarrier(
				vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer,
				{},
				{}, {}, m_barriers
			);
		}

		// コピー
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
			region.imageSubresource = vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor,0/*request.mipLevels*/, 0, request.layerCount);
			region.imageOffset = 0;
			region.imageExtent = request.extent;

			commandBuffer.copyBufferToImage(request.source, request.dest, vk::ImageLayout::eTransferDstOptimal, region);

		}

		// バリア
		m_barriers.clear();
		for (auto& request : frame.requests) {

			vk::ImageAspectFlags flags{};
			if (TextureFormatUtility::HasColor(request.format)) flags |= vk::ImageAspectFlagBits::eColor;
			if (TextureFormatUtility::HasDepth(request.format)) flags |= vk::ImageAspectFlagBits::eDepth;
			if (TextureFormatUtility::HasStencil(request.format)) flags |= vk::ImageAspectFlagBits::eStencil;

			vk::ImageMemoryBarrier barrier;
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
			barrier.oldLayout = vk::ImageLayout::eUndefined;
			barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = request.dest;
			barrier.subresourceRange.aspectMask = flags;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = request.mipLevels;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = request.layerCount;
			m_barriers.push_back(barrier);

		}
		if (!m_barriers.empty()) {
			commandBuffer.pipelineBarrier(
				vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eAllGraphics,
				{},
				{}, {}, m_barriers
			);
		}

		m_frames.next();
		m_frames.current().clear();

		commandList->popMarker();

	}

}