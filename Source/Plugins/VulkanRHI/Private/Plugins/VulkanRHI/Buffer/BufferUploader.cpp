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
	void BufferUploader::add(BlobView blob, vk::raii::Buffer& dest, size_t offset, vk::AccessFlagBits postAccessFlags) {

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
		request.postAccessFlags = postAccessFlags;

		block.blob.append(blob.data(), blob.size());

	}

	void BufferUploader::add(const Buffer::CopyFunc& func, size_t size, vk::raii::Buffer& dest, size_t offset, vk::AccessFlagBits postAccessFlags) {

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
		request.postAccessFlags = postAccessFlags;

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

		// 一次バッファ生成
		block.blob.reserve(m_blockSize);

	}

	//! @brief アップロードバッファを縮小する
	void BufferUploader::shurink() {

		auto& frame = m_frames.current();

		if (frame.blocks.empty()) return;

		frame.blocks.pop_back();

	}

	void BufferUploader::update(vk::raii::CommandBuffer& commandBuffer,bool useDebugMarker) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		auto& device = m_rhi.getDevice();

		for (s32 i = 0; i <= frame.blockIndex; ++i) {
			auto& block = frame.blocks.at(i);

			void* data = block.memory.mapMemory(0, block.blob.size());

			memcpy_s(data, m_blockSize, block.blob.data(), block.blob.size());

			block.memory.unmapMemory();
		}

		if(useDebugMarker) commandBuffer.debugMarkerBeginEXT("BufferUploader");

		// blocks 事前バリア設定(COMMON or GENERIC_READ > COPY_SOURCE)
		m_barriers.clear();
		for (auto& request : frame.requests) {
			auto& barrier = m_barriers.emplace_back();
			barrier = vk::BufferMemoryBarrier();
			barrier.srcAccessMask = vk::AccessFlagBits::eNone; // TODO : 確認
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.buffer = request.dest;
			barrier.offset = request.destOffset;
			barrier.size = request.size;
		}
		// requests  事前バリア設定 (COMMON > COPY_DEST)
		for (auto& block : frame.blocks) {
			auto& barrier = m_barriers.emplace_back();
			barrier = vk::BufferMemoryBarrier();
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.buffer = block.buffer;
			barrier.offset = 0;
			barrier.size = block.blob.size();
		}
		if (!m_barriers.empty()) {
			commandBuffer.pipelineBarrier(
				vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer,
				{},
				{}, m_barriers, {}
			);
		}


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
			barrier.dstAccessMask = request.postAccessFlags;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.buffer = request.dest;
			barrier.offset = request.destOffset;
			barrier.size = request.size;
		}
		// requests  事前バリア設定は暗黙的な昇格を使用 (COMMON > COPY_DEST)
		for (auto& block : frame.blocks) {
			auto& barrier = m_barriers.emplace_back();
			barrier = vk::BufferMemoryBarrier();
			barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
			barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.buffer = block.buffer;
			barrier.offset = 0;
			barrier.size = block.blob.size();
		}
		if (!m_barriers.empty()) {
			commandBuffer.pipelineBarrier(
				vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer,
				{},
				{}, m_barriers, {}
			);
		}

		// バッファを縮小
		frame.clear();

		if (useDebugMarker) commandBuffer.debugMarkerEndEXT();

	}

}