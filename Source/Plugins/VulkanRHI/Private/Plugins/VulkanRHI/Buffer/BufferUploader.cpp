//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Buffer/BufferUploader.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>

namespace ob::rhi::vulkan
{

	//! @brief  コンストラクタ
	BufferUploader::BufferUploader(VulkanRHI& rhi, size_t blockSize)
		: m_rhi(rhi)
	{
		m_blockSize = align_up(blockSize, 256);
		m_frames.resize(4);
		for (s32 i = 0; i < m_frames.size(); ++i) {
			size_t requestNum = m_blockSize / 256;
			m_frames.at(i).requests.reserve(requestNum);
		}
	}

	//! @brief  アップロード要素を追加
	void BufferUploader::add(BlobView blob, vk::raii::Buffer& dest, size_t offset) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		// バッファが足りない場合は拡張
		if (!frame.available(blob.size())) {
			extend();
		}

		auto& block = frame.block();

		auto& request = frame.requests.emplace_back();

		request.source = block.buffer;
		request.dest = *dest;
		request.sourceOffset = block.blob.size();
		request.destOffset = offset;
		request.size = blob.size();

		block.blob.append(blob.data(), blob.size());

	}

	void BufferUploader::add(const Buffer::CopyFunc& func, size_t size, vk::raii::Buffer& dest, size_t offset) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		// バッファが足りない場合は拡張
		if (!frame.available(size)) {
			extend();
		}

		auto& block = frame.block();

		auto& request = frame.requests.emplace_back();

		request.source = block.buffer;
		request.dest = dest;
		request.sourceOffset = block.blob.size();
		request.destOffset = offset;
		request.size = size;

		block.blob.resize(block.blob.size() + size);

		func(block.blob.data() + request.sourceOffset);
	}

	//! @brief アップロードバッファを拡大する
	void BufferUploader::extend() {

		auto& frame = m_frames.current();

		frame.blockIndex++;

		// 確保済みバッファがある場合はインデックスだけ進める
		if (frame.blockIndex < frame.blocks.size()) {
			return;
		}

		auto& block = frame.blocks.emplace_back();

		auto& device = m_rhi.getDevice();
		auto allocationCallbacks = m_rhi.getAllocationCallbacks();

		vk::BufferCreateInfo info;
		info.size = m_blockSize;
		info.usage = vk::BufferUsageFlagBits::eTransferSrc;
		info.sharingMode = vk::SharingMode::eExclusive;


		block.buffer = device.createBuffer(info, allocationCallbacks);

		auto requirements = block.buffer.getMemoryRequirements();

		VkMemoryAllocateInfo allocInfo = m_rhi.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		block.memory = device.allocateMemory(allocInfo, allocationCallbacks);
		block.buffer.bindMemory(block.memory, 0);

		// 一次バッファ生成
		block.blob.reserve(m_blockSize);

	}

	//! @brief アップロードバッファを縮小する
	void BufferUploader::shurink() {

		auto& frame = m_frames.current();

		if (frame.blocks.empty()) return;

		frame.blocks.pop_back();

	}

	void BufferUploader::update(vk::raii::CommandBuffer& commandBuffer) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		auto& device = m_rhi.getDevice();

		for (s32 i = 0; i <= frame.blockIndex; ++i) {
			auto& block = frame.blocks.at(i);

			void* data = block.memory.mapMemory(0, block.blob.size());

			memcpy_s(data, m_blockSize, block.blob.data(), block.blob.size());

			block.memory.unmapMemory();
		}

		//::PIXBeginEvent(&commandList, PIX_COLOR_DEFAULT, L"BufferUploader");

		// blocks 事前バリア設定は暗黙的な昇格を使用 (COMMON or GENERIC_READ > COPY_SOURCE)
		// requests  事前バリア設定は暗黙的な昇格を使用 (COMMON > COPY_DEST)

		// コピー
		for (auto& request : frame.requests) {
			// NOTE regionは複数指定できるのでSourceとDestが同じ場合はコマンドをまとめることができる
			vk::BufferCopy region(request.sourceOffset, request.destOffset, request.size);
			commandBuffer.copyBuffer(request.source, request.dest, region);
		}

		// blocks 事前バリア設定は暗黙的な降格を使用 (COPY_SOURCE > COMMON) ※ExecuteCommandLists後
		// TODO 同じリソースが複数回使用される場合は、バリアをまとめて実行する
		m_barriers.clear();
		for (auto& request : frame.requests) {
			auto& barrier = m_barriers.emplace_back();

			barrier = vk::BufferMemoryBarrier();
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eNone;
			//barrier.srcQueueFamilyIndex;
			//barrier.dstQueueFamilyIndex;
			barrier.buffer = request.dest;
			barrier.offset;
			barrier.size;

		}
		if (!m_barriers.empty()) {
			//commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,vk::QueryPipelineStatisticFlagBits(-1),m_barriers,)
			//
			//vkCmdPipelineBarrier(commandBuffer,
			//	VK_PIPELINE_STAGE_TRANSFER_BIT,
			//	VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			//	0,
			//	0, nullptr,
			//	m_barriers.size(), m_barriers.data(),
			//	0, nullptr);
		}

		// バッファを縮小
		frame.clear();

		//::PIXEndEvent(&commandList);

	}

}